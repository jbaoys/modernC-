#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "sensors.server"
#define CLIENT_PATH "sensor.client"
using namespace std;

int main(int argc, char** argv){

    int client_sock, rc, len, sensorID;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char buf[256];
    string cliStr;
    if (argc>1) {
        if (!strcmp(argv[1], "STOP")) {
            sensorID = -1;
        } else {
            sensorID = atoi(argv[1]);
        }
    } else {
        sensorID = 1;
    }
    cliStr = CLIENT_PATH + to_string(sensorID);

    for(;;)
    {
        usleep(50000);
        memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
        memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
        // Create a UNIX domain stream socket
        client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (client_sock == -1) {
            cout << "FAIL TO CREATE SOCKET\n";
            exit(1);
        }

        client_sockaddr.sun_family = AF_UNIX;
        strcpy(client_sockaddr.sun_path, cliStr.c_str());
        len = sizeof(client_sockaddr);

        unlink(cliStr.c_str());
        rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
        if (rc == -1){
            cout << "BIND ERROR !!!\n";
            close(client_sock);
            exit(1);
        }

        // Set up the UNIX sockaddr structure
        server_sockaddr.sun_family = AF_UNIX;
        strcpy(server_sockaddr.sun_path, SERVER_PATH);
        rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
        if(rc == -1){
            cout << "CONNECT ERROR !!!\n";
            close(client_sock);
            exit(1);
        }
        // Copy the data to the buffer and send out
        string cmd = to_string(sensorID);
        if (sensorID == -1) {
            cmd = "STOP1234567890";
        }
        strcpy(buf, cmd.c_str());
        rc = send(client_sock, buf, strlen(buf), 0);
        if (rc == -1) {
            cout << "SEND ERROR !!!\n";
            close(client_sock);
            exit(1);
        }
        else {
            if (sensorID == -1) {
                cout << "STOP THE SERVER REQUEST IS SENT!!!\n";
                break;
            }
            cout << "Reading sensor[" << cmd <<"]\n";
        }

        // Read the sensor sent from the server
        memset(buf, 0, sizeof(buf));
        rc = recv(client_sock, buf, sizeof(buf), 0);
        if (rc == -1) {
            cout << "RECV ERROR !!!\n";
            close(client_sock);
            exit(1);
        }
        else {
            cout << "Senor[" << sensorID << "] = " << buf << endl;
        }
        close(client_sock);
    }

    return 0;
}
