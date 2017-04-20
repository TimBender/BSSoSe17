#include<fstream>
#include<sstream>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <wait.h>
#include <time.h>
#include <ctime>
#include <vector>
using namespace std;

#define BUFFER_BIT 510

int main(int argc, char** argv) {
    const string file_name = "init.txt";
    int status;
    bool isAutoMode = false;
    size_t step_counter = 0;
    vector<int> m_register = vector<int>(2, 0); // val1= Integerregister; val2= Program Counter
    fstream init_data(file_name.c_str());
    fstream register_data("register.txt");

    string buffer1, buffer2;
    if (init_data.good()) {

        /*Ausgabe des init Programs*/
        while (getline(init_data, buffer1, ' '), getline(init_data, buffer2)) {
            cout << buffer1 << ' ' << buffer2 << endl;
        }
    }
    /*Pipe erstellen*/
    int fd[2];

    pipe(fd); // in C automatisch Ref zum 1.obj

    /*Kopie von sich selbst erstellen*/
    int pm_id;
    char command[BUFFER_BIT];
    clock_t tstart = clock(); // start


    switch (pm_id = fork()) {
        case -1:
            cerr << "ERROR_forking failed.\n";
            break;
        case 0: /*child*/
        {
            //                while(1){
            //                    
            //                }
            read(fd[0], command, BUFFER_BIT);
            string cmd = command;
            //Stoppen
            if (cmd == "B" || cmd == "Block") {
                cout << "Process blocked." << endl;
                //kill(getpid(), SIGSTOP);
            }
            //Weitermachen
            if (cmd == "U" || cmd == "Unblock") {
                cout << "Process unblocked." << endl;
                //                    kill(getpid(), SIGCONT);
            }

            //Modus wechseln
            if (cmd == "M" || cmd == "Mode") {
                cout << "Mode changed." << endl;
                isAutoMode = true;

            }

            //Aufgabe 3
            /*Schreibe Programmverlauf in init*/
            if (cmd == "S" || cmd == "R" || cmd == "A" || cmd == "D" || cmd == "E" || cmd == "B") {
                init_data << command << endl;
            }

            //nächsten Befehl ausführen
            if ((cmd == "S" || cmd == "Step") && isAutoMode == true) {
                cout << "Next step. Step " << ++step_counter << endl;

            }
            
            int val = 0;
            if (cmd.find("A ") != string::npos) {

                stringstream(cmd.substr(cmd.find(' ') + 1, cmd.size() - 1))>> val;
                m_register[0] = m_register[0] + val;
            }
            if (cmd.find("D ") != string::npos) {
                int val = 0;
                stringstream(cmd.substr(cmd.find(' ') + 1, cmd.size() - 1))>> val;
                m_register[0] = m_register[0] - val;
            }

            if (cmd.find("S ") != string::npos) {
                stringstream(cmd.substr(cmd.find(' ') + 1, cmd.size() - 1))>> val;
                m_register[0] = val;
            }
            
            if (cmd == "E"){
                cout << "Process terminated" << endl;
                exit(0);
            }
            if (cmd.find("R ") != string::npos) {
                int newProgram = fork();

                switch (newProgram) {
                    case -1:
                        cerr << "ERROR_fork failed";
                        exit(1);
                    case 0:
                    {
                        //Oeffne Datei n und springe zurueck zu init
                        string name = cmd.substr(cmd.find(' ') + 1, cmd.size() - 1) + ".txt";
                        cout << cmd.substr(cmd.find(' ') + 1, cmd.size() - 1) << endl;
                        fstream new_file(name.c_str());
                        exit(0);
                    }
                        break;
                    default:
                    {
                        //Init
                        register_data << m_register[0] << ';' << m_register[1] << endl;
                        wait(&status);

                        if (register_data.good()) {
                            //Register wieder zuruecklesen

                            while (!register_data.eof()) {
                                getline(register_data, buffer1, ';');
                                getline(register_data, buffer2);
                                stringstream(buffer1) >> m_register[0];
                                stringstream(buffer1) >> m_register[1];
                                cout << m_register[0] << ' ' << m_register[1] << endl;
                                //
                            }
                        }

                    }
                        break;
                }

            }
            //Ausgabe der Werte des Prozesses
            if (cmd == "P" || cmd == "Print") {
                int rep_id = fork();
                switch (rep_id) {
                    case 0: //Reporter
                        cout << "RUNNING PROCESS:\n";
                        system("ps aux | less"); //running processes
                        //system("ps -o pid,pgid,time,etime,group,ppid,user,vsz,cmd");
                        exit(0);
                        break;
                    default:
                        wait(&status);
                        break;
                }
            }
        }
            break;
        default: /*parent*/
        {
            cout << "Command: ";
            cin.getline(command, BUFFER_BIT);
            //cin>>command;

            string buffer = command;
            //Beenden
            if (buffer == "Q" || buffer == "Quit") {
                cout << "Durchlaufzeit: " << (clock() - tstart) / CLOCKS_PER_SEC << '\n';
                return 0;
                //kill(getppid(), SIGTERM);
            }

            /*write to pipe*/
            write(fd[1], command, BUFFER_BIT);
            wait(&status);
            break;
        }
    }
    cout << "\n";
    //ofstream init_data(file_name);




    exit(0);
    return 0;
}
