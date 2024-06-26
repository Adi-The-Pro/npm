#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class Server_socket
{
    fstream file;

    int PORT;

    char fileName[100];
    char file_buffer[2000];

    int general_socket_descriptor;
    int new_socket_descriptor;

    struct sockaddr_in address;
    int address_length;

public:
    Server_socket()
    {
        create_socket();
        PORT = 8050;

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        address_length = sizeof(address);

        bind_socket();
        set_listen_set();
        accept_connection();

      
        // Open the file
    cout << "Enter text file name to send : \n";
    cin >> fileName;
    file.open(fileName, ios::in);

    // Check if the file is opened successfully
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file.\n";
        exit(EXIT_FAILURE);
    }

    // Read the contents of the file into file_buffer
    file.read(file_buffer, sizeof(file_buffer));

    // Close the file
    file.close();

    cout << "FILE CONTENTS TO SEND : " << file_buffer << endl;
       
    }

    void create_socket()
    {
        if ((general_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("[ERROR] : Socket failed");
            exit(EXIT_FAILURE);
        }
        cout << "[LOG] : Socket Created Successfully.\n";
    }

    void bind_socket()
    {
        if (bind(general_socket_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("[ERROR] : Bind failed");
            exit(EXIT_FAILURE);
        }
        cout << "[LOG] : Bind Successful.\n";
    }

    void set_listen_set()
    {
        if (listen(general_socket_descriptor, 3) < 0)
        {
            perror("[ERROR] : Listen");
            exit(EXIT_FAILURE);
        }
        cout << "[LOG] : Socket in Listen State (Max Connection Queue: 3)\n";
    }

    void accept_connection()
    {
        if ((new_socket_descriptor = accept(general_socket_descriptor, (struct sockaddr *)&address, (socklen_t *)&address_length)) < 0)
        {
            perror("[ERROR] : Accept");
            exit(EXIT_FAILURE);
        }
        cout << "[LOG] : Connected to Client.\n";
    }

    void transmit_file()
    {
        // Get the size of the file_buffer
        int file_size = strlen(file_buffer);

        cout << "[LOG] : Transmission Data Size " << file_size << " Bytes.\n";
        cout << "[LOG] : Sending...\n";

        // Send the contents of the file_buffer
        int bytes_sent = send(new_socket_descriptor, file_buffer, file_size, 0);
        cout << "[LOG] : Transmitted Data Size " << bytes_sent << " Bytes.\n";

        cout << "[LOG] : File Transfer Complete.\n";
    }
};

int main()
{
    Server_socket S;
    S.transmit_file();
    return 0;
}