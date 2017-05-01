#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <wait.h>
#include <time.h> 
#include <ctime>
using namespace std;

#define BUFFER_BIT 510

int main(int argc, char** argv) {
    /*Pipe erstellen*/
    int fd[2];
    pipe(fd); // in C automatisch Ref zum 1.obj

    char input = ' ';
    /*Kopie von sich selbst erstellen*/
    int pm_id;
    char command[BUFFER_BIT];

    clock_t tstart = clock(); // start 
    while (true) {

        switch (pm_id = fork()) {
            case -1:
                cerr << "ERROR_forking failed.\n";
                break;

            case 0: /*child*/
            {
                cin>>command;
                /*write to pipe*/
                write(fd[1], command, BUFFER_BIT);

                read(fd[0], command, BUFFER_BIT);

                string n = command;
                if (n == "Q" || n == "Quit") {
                    cout << "Durchlaufzeit: " << (clock() - tstart) / CLOCKS_PER_SEC << '\n';
                    kill(getppid(), SIGUSR1);

                }
                if (n == "P" || n == "Print") {
                    int rep_id = fork();

                    switch (rep_id) {
                        case 0: //Reporter
                            cout << "RUNNING PROCESS:\n";
                            system("ps aux | less"); //running processes
                            //system("ps -o pid,pgid,time,etime,group,ppid,user,vsz,cmd");
                            exit(0);
                            break;
                        default:
                            wait(0);
                            break;
                    }
                }
            }
                break;

            default: /*parent*/
            {
                wait(0);
                break;
            }
        }
        cout << "\n";
    }
    exit(0);
    return 0;
}

