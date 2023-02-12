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

using namespace std;
struct SensorData;
static int SensorIDPool = 0;
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

struct SensorData{
    float latestValue;
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
    unordered_map<int, SensorData> m_Sensors;
    mutex m_Mtx;

public:
    /**
     * Constructor
     */
    SensorQuerySrv(vector<GenericSensor*> sensors) {
        for (auto & s : sensors) {
            m_Sensors[s->getID()] = SensorData(s);
            m_Sensors[s->getID()].setReturnData(); // hook up with the m_Data
        }
    };
    /**
     * Destructor
     */
    virtual ~SensorQuerySrv() {};

    /**
     * retrieve function
     */
    float readSensor(int sen) {
        if (m_Sensors.find(sen) == m_Sensors.end()) {
            return -1.0; // Non exist sensor
        } else {
            float value;
            lock_guard<mutex> guard(m_Sensors[sen].sensor->mx);
            value = m_Sensors[sen].latestValue;
            return value;
        }
    };
};

int main()
{

    thread senThread1(runSen1);
    thread senThread2(runSen2);
    SensorQuerySrv server(mySensors);
    for (;;)
    {
        sleep(1);
        cout << "1: " << server.readSensor(1) << endl;
        cout << "2: " << server.readSensor(2) << endl;
    }
    senThread1.join();
    senThread2.join();
    return 0;
}
