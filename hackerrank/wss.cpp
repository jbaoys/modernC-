/**
 * Please create a C++ class implementing sensor query server, with the following characteristics:
 *
 * 1. The server is a process;
 * 2. The server queries the sensors periodically, each sensor with its own frequency
 * 3. The list of the sensors is supplied as the constructor/initializer
 *    (on your choice) as a list of structures (or objects of some class, on
 *    your choice), and doesn't change during the server's object life.
 *    Each element of the array contains the sensor access information
 *    (pointer or reference to the object of the GenericSensor class) and
 *    query periodicity, in milliseconds. Please consider the GenericSensor
 *    as an abstract class implementing
 *    single method: float GenericSensor::getValue();
 * 4. The measured data types may vary, but is always a floating point
 *    value (e.g. temperature, pressure, velocity, etc.)
 * 5. Specific implemetation of low-level access to the sensors is
 *    separate and out of scope of this problem. You can safely assume that
 *    the access to the sensors is thread-safe.
 * 6. The data asquired from the sensors is stored internally in the
 *    server object and provided on demand to the clients.
 * 7. The server provides interface for the sensor data retrieval. The
 *    interface must be designed in the way allowing the data receive by one
 *    or more pther processes. Please use IPC of your choice for this
 *    purpose.
 *    The provided data is result of the latest query of the sensor.
 * 8. Please use coding and comment conventions of your choice
 * 9. The code is not required to be running, but must be complete enough
 *    to be understood.
 * 10. Please avoid usage of existing implementations of the servers, the
 *    only allowed external template library is STL.
 */
#include <bits/stdc++.h>
#include <thread>
#include <unistd.h>
#include <functional>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH  "sensors.server"

using namespace std;
struct SensorData;
static int SensorIDPool = 0;
/**
 * The following code are to be mock the real sensor readings.
 */
/**
 * @class   Sensor
 * @brief   This is an abstract sensor
 */
class GenericSensor
{
private:
    int m_ID;
public:
    GenericSensor() : m_ID(++SensorIDPool), m_Delay(1000), m_Data(nullptr){};
    virtual ~GenericSensor(){};

    mutex mx;
    /**
     * Return a value of read from hareware.
     * Must be implemented according to the type of sensor.
     */
    virtual float getValue() = 0;
    int m_Delay;
    SensorData* m_Data;
    int getID() { return m_ID; };
};

/**
 * This struct is used to contain the reading value from a sensor
 */
struct SensorData{
    float latestValue;          /// Is used to contain the latest reading value
    GenericSensor* sensor;
    SensorData() : latestValue(0.0), sensor(nullptr) {};
    SensorData(GenericSensor* pSen) : latestValue(0.0), sensor(pSen) {
    };
    void setReturnData() {
        sensor->m_Data = this;
    };
};

/**
 * @class   Sensor1
 * @brief   mimic sensor1 behavior
 */
class Sensor1 : public GenericSensor
{
public:
    Sensor1() {};
    virtual ~Sensor1() {};
    float getValue() {
        float val = (float)(rand() % 1000)/2.0;
        // Save the reading value back to a container
        // mimic a call back
        lock_guard<mutex> guard(mx);
        m_Data->latestValue = val;
        return val;
    };
};

/**
 * @class   Sensor2
 * @brief   mimic sensor2 behavior
 */
class Sensor2 : public GenericSensor
{
public:
    Sensor2() {};
    virtual ~Sensor2() {};
    float getValue() {
        float val = (float)(rand() % 10000)/5.0;
        lock_guard<mutex> guard(mx);
        m_Data->latestValue = val;
        return val;
    };
};

Sensor1 obj1;
Sensor2 obj2;
vector<GenericSensor*> mySensors = {&obj1, &obj2};

/**
 * Periodically running function
 * These two function are used to mimic perorically reading process
 * from a sensor.
 */
void runSen1() {
    for (;;) {
        usleep(1000000);
        float value = obj1.getValue();
    }
};
void runSen2() {
    for (;;) {
        usleep(2000000);
        float value = obj2.getValue();
    }
};

/**
 * @class   SensorQuerySrv
 * @brief   This is a sensor query server.
 *
 */
class SensorQuerySrv
{
private:
    unordered_map<int, SensorData> m_Sensors;   /// sensor container
    mutex m_Mtx;

public:
    /**
     * Constructor
     */
    SensorQuerySrv(vector<GenericSensor*> sensors) {
        for (auto & s : sensors) {
            m_Sensors.emplace(s->getID(), SensorData(s));
            m_Sensors[s->getID()].setReturnData(); // hook up with the m_Data
        }
    };

    /**
     * Destructor
     */
    virtual ~SensorQuerySrv() {};

    /**
     * retrieve function
     * This function is provided to multiple threads access.
     */
    float readSensor(int sen) {
        if (m_Sensors.find(sen) == m_Sensors.end()) {
            return -1.0; // Non exist sensor
        } else {
            float value;
            // protect the reading value
            lock_guard<mutex> guard(m_Sensors[sen].sensor->mx);
            value = m_Sensors[sen].latestValue;
            return value;
        }
    };
};

int main()
{
    /**
     * The server is implementd by UNIX domain socket.
     * It will listen the connections. As soon as there is a connection,
     * it will call the above class object with sensor ID, and return
     * the read value to the client.
     */
    int server_sock, client_sock, rc;
    socklen_t len;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    int backlog = 10, sensorID;
    float retVal;
    string cmd;
    thread senThread1(runSen1);
    thread senThread2(runSen2);
    SensorQuerySrv server(mySensors);

    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, 256);

    // Create a domain stream socket
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1){
        cout << "SOCKET ERROR WHEN IT IS CREATED\n";
        exit(1);
    }

    // Set up the UNIX sockaddr structure
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCK_PATH);
    len = sizeof(server_sockaddr);
    unlink(SOCK_PATH);
    rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
    if (rc == -1){
        cout << "FAIL TO BIND SERVER !!!\n";
        close(server_sock);
        exit(1);
    }

    // listen to connections
    rc = listen(server_sock, backlog);
    if (rc == -1){
        cout << "LISTEN ERROR  !!!\n";
        close(server_sock);
        exit(1);
    }
    cout << "socket listening...\n";

    while (1) {
        // accepting the client request
        client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);
        if (client_sock == -1){
            cout << "ACCEPT ERROR !!!\n";
            close(server_sock);
            close(client_sock);
            exit(1);
        }

        // get client request cmd
        memset(buf, 0, 256);
        bytes_rec = recv(client_sock, buf, sizeof(buf), 0);
        if (bytes_rec == -1){
            cout << "RECV ERROR !!!\n";
            close(server_sock);
            close(client_sock);
            exit(1);
        }
        else {
            cmd = buf;
            // parse the request
            sensorID = stoi(cmd);
            cout << "READ Senor[" <<  buf  << "]" << endl;
        }
        // set data back
        retVal = server.readSensor(sensorID);
        cmd = to_string(retVal);
        memset(buf, 0, 256);
        strcpy(buf, cmd.c_str());
        rc = send(client_sock, buf, strlen(buf), 0);
        if (rc == -1) {
            cout << "SEND ERROR !!!\n";
            close(server_sock);
            close(client_sock);
            exit(1);
        }
        else {
            cout << "Data sent!\n";
        }
    }
    close(server_sock);
    close(client_sock);
    senThread1.join();
    senThread2.join();
    return 0;
}
