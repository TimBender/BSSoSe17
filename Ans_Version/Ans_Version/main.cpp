#include"SimulatedProcess.h"

#include<ctime>
#include<cstdlib>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<list>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#include <string>

using namespace std;

const int BUFFER_BIT = 510;
const float QUANTUM = 0.05; //quantum for ROUND ROBIN SCHEDULING

int main(int argc, char** argv) {
    int status;

    //2 queues for scheduling
    list<SimulatedProcess> blocked_processes;
    list<SimulatedProcess> non_blocked_processes;

    // starts with: Debug Mode
    bool isAutoMode = false;

    //two registers 
    int* integer_register = NULL;
    int* program_counter = NULL;
    int current_int_reg_value = 0;
    int pc_value = 0;

    //starting time: processor time
    time_t starting_time = (double) clock() / CLOCKS_PER_SEC;

    //count current steps in total
    size_t step_counter = 0;

    //string command;
    char cmd;

    //for n value in the given command
    string file = "";
    int value;

    char s_cmd = ' ';
    int next_idx = 0;
    time_t process_start_time;
    bool timerIsOn = false;

    SimulatedProcess *currently_simulated_process, buffer; //currently executed simulated process, buffer for process switching
    SimulatedProcess sim_process(getppid(), "init"); //initial program: init

    /*create pipe*/
    int fd[2];
    pipe(fd);

    /*create copy*/
    int pm_id;
    char command[BUFFER_BIT];
    
    switch (pm_id = fork()) {
        case -1: //forking failed
            cerr << "ERROR_forking failed.\n";
            break;

        case 0: //child
        {
            //read from pipe
            read(fd[0], command, BUFFER_BIT);
            string cmd = command;

            currently_simulated_process = &sim_process; //program start: init is the initially executed program

            //ROUND- ROBIN SCHEDULING: set starting/ reset process timer
            if (!timerIsOn) {
                process_start_time = (double) clock() / CLOCKS_PER_SEC;
                timerIsOn = true;
            }

            if (((double) clock() / CLOCKS_PER_SEC) - process_start_time >= QUANTUM) // quantum exhausted
            {
                if (!non_blocked_processes.empty()) {
                    // test: cout << "TIME: " << ((double)clock() / CLOCKS_PER_SEC) - process_start_time << '\t' << "Quantum: " << QUANTUM;
                    non_blocked_processes.push_back(*currently_simulated_process); //current process is queued at the far end of the line 
                    buffer = non_blocked_processes.front();
                    currently_simulated_process = &buffer; //switch with oldest queued (ready-to-execute) process 
                    non_blocked_processes.pop_front(); //remove from queue
                    timerIsOn = false; //to reset process quantum timer
                }
            }

            /*User, Commandant, Processmanager commands*/

            //Reporter process: print out process data
            if (cmd == "P" || cmd == "Print") {
                int rep_id = fork();
                switch (rep_id) {
                    case 0: //Reporter
                        cout << "\n___________________________________________________________________________________________________________\n";
                        cout << "The current system state is as follows :\n";
                        cout << "_____________________________________________________________________________________________________________\n\n";

                        cout << "Current time: " << (double) clock() / CLOCKS_PER_SEC << "\n\n";
                        cout << "-------------------------------------------------------------------------------------------------------------\n";
                        cout << "RUNNING PROCESS:\n";
                        cout << sim_process << '\n';
                        cout << "-------------------------------------------------------------------------------------------------------------\n";
                        cout << "BLOCKED PROCESSES:\n";
                        for (auto& process : blocked_processes) {
                            cout << process;
                        }
                        cout << "-------------------------------------------------------------------------------------------------------------\n";
                        cout << "PROCESSES READY TO EXECUTE:\n";
                        for (auto& process : non_blocked_processes) {
                            cout << process;
                        }
                        cout << endl;

                        //system("ps aux | less"); //running processes: ok?
                        exit(0);
                        break;
                    default: //parent
                        wait(&status);
                        break;
                }
            }

            //switch modes: debug to auto
            if (cmd == "M" || cmd == "Mode") {
                cout << "Mode changed." << endl;
                isAutoMode = true;
            }

            //unblock oldest blocked process: scheduling descision= process switch: current simulated process will be replaced by unblocked process
            if (cmd == "U" || cmd == "Unblock") {
                blocked_processes.front().unblock(); //change status from blocked to unblock
                non_blocked_processes.push_front(*currently_simulated_process); //switched process will be queued as the next executable process in line

                //delete process from the blocked processes queue
                buffer = blocked_processes.front();
                blocked_processes.pop_front();

                currently_simulated_process = &buffer; //current process equals the unblocked process
            }

            //Step: executed next command line
            if ((cmd == "S" || cmd == "Step") && isAutoMode == false) {
                //all processes are blocked
                if (currently_simulated_process->getIsBlocked()) { // Commandant has to UNBLOCK process
                    cout << "Sorry, this simulated process is currently blocked. Unblock it and try again.\n";
                    exit(0); //return to parent 
                }

                if (next_idx < currently_simulated_process->getInstructionMemory().size()) //executable command line exists
                {
                    cout << "Next step. Step " << ++step_counter << endl; //print out current step 
                    ++next_idx;

                    auto instruction = currently_simulated_process->getInstructionMemory().at(next_idx - 1); //get next command line

                    /*extract n from command line*/
                    if (instruction[0] != 'E' && instruction[0] != 'B') { //case: no n in command line
                        if (toupper(instruction[0]) != 'R') stringstream(instruction.substr(1, instruction.size() - 1)) >> value; // n= integer value
                        else stringstream(instruction.substr(2, instruction.size() - 1)) >> file; // n= file name
                    }

                    /*simulated CPU instruction set*/
                    switch (toupper(instruction[0])) {
                        case 'S': //Init integer register	
                            integer_register = &value;
                            break;

                        case 'A': //Add to integer register value
                            current_int_reg_value += value;
                            integer_register = &current_int_reg_value;
                            break;

                        case 'D': //Subtract from integer register value
                            current_int_reg_value -= value;
                            integer_register = &current_int_reg_value;
                            break;

                        case 'B': //Block simulated process
                            currently_simulated_process->block(); // switch process state to blocked
                            blocked_processes.push_back(*currently_simulated_process); //add to blocked processes queue

                            if (!non_blocked_processes.empty()) { //there is at least one queued process ready to be executed
                                buffer = non_blocked_processes.front(); // scheduling descision: oldest queued process ready to be executed= current sim. process
                                currently_simulated_process = &buffer;
                                next_idx = 0;
                                non_blocked_processes.pop_front(); // remove from queue
                            } else currently_simulated_process = NULL; //no non-blocked processes available 

                            break;

                        case 'E': //Beende sim. Prozess
                            cout << "Terminated simulated process.\n";
                            exit(0); // back to parent
                            break;

                        case 'R': //neuer sim.Prozess + Datei ausfuehren
                        {
                            SimulatedProcess new_process(0, file);
                            pc_value++;
                            program_counter = &pc_value;

                            new_process.setIntegerRegsiter(*integer_register); //speichere Integerregister
                            new_process.setProgramCounter(*program_counter); //speichere Programcounter

                            non_blocked_processes.push_back(new_process); //aufnehmen in Schlange der nicht blockierten Prozesse				 
                        }
                            break;

                        default:
                            cout << "Sorry, unknown simulated CPU command. Please try again.\n";
                            break;
                    }
                } else exit(0); //alle Programm Instruktionen ausgefuehrt
            }

            // Automode is on
            if (isAutoMode) {
                for (size_t i = 0; i < currently_simulated_process->getInstructionMemory().size(); i++) {
                    auto instruction = currently_simulated_process->getInstructionMemory().at(i); //get next command line

                    /*extract n from command line*/
                    if (instruction[0] != 'E' && instruction[0] != 'B') { //case: no n in command line
                        if (toupper(instruction[0]) != 'R') stringstream(instruction.substr(1, instruction.size() - 1)) >> value; // n= integer value
                        else stringstream(instruction.substr(2, instruction.size() - 1)) >> file; // n= file name
                    }

                    /*simulated CPU instruction set*/
                    switch (toupper(instruction[0])) {
                        case 'S': //Init integer register	
                            integer_register = &value;
                            break;

                        case 'A': //Add to integer register value
                            current_int_reg_value += value;
                            integer_register = &current_int_reg_value;
                            break;

                        case 'D': //Subtract from integer register value
                            current_int_reg_value -= value;
                            integer_register = &current_int_reg_value;
                            break;

                        case 'B': //Block simulated process
                            currently_simulated_process->block(); // switch process state to blocked
                            blocked_processes.push_back(*currently_simulated_process); //add to blocked processes queue

                            if (!non_blocked_processes.empty()) { //there is at least one queued process ready to be executed
                                buffer = non_blocked_processes.front(); // scheduling descision: oldest queued process ready to be executed= current sim. process
                                currently_simulated_process = &buffer;
                                next_idx = 0;
                                non_blocked_processes.pop_front(); // remove from queue
                            } else currently_simulated_process = NULL; //no non-blocked processes available 

                            break;

                        case 'E': //Beende sim. Prozess
                            cout << "Terminated simulated process.\n";
                            exit(0);
                            break;

                        case 'R': //neuer sim.Prozess + Datei ausfuehren
                        {
                            SimulatedProcess new_process(0, file);
                            pc_value++;
                            program_counter = &pc_value;

                            new_process.setIntegerRegsiter(*integer_register); //speichere Integerregister
                            new_process.setProgramCounter(*program_counter); //speichere Programcounter

                            non_blocked_processes.push_back(new_process); //aufnehmen in Schlange der nicht blockierten Prozesse				 
                        }
                            break;

                        default:
                            cout << "Sorry, unknown simulated CPU command. Please try again.\n";
                            break;
                    }
                }
                exit(0);
            }

        }
            break;

        default: //parent
        {
            
           
            cout << "Command: ";
            cin.getline(command, BUFFER_BIT);

            string buffer = command;

            //terminate
            if (buffer == "Q" || buffer == "Quit") {
                float median_durchlaufzeit = 0.0f; //average elapsed (processor) time

                for (auto process : non_blocked_processes) {
                    median_durchlaufzeit += (process.get_cpu_time() - process.getStartingTime());
                }
                for (auto process : blocked_processes) {
                    median_durchlaufzeit += (process.get_cpu_time() - process.getStartingTime());
                }

                median_durchlaufzeit = median_durchlaufzeit / (blocked_processes.size() + non_blocked_processes.size());
                cout << "Durchlaufzeit: " << median_durchlaufzeit << '\n';
                return 0;
            }

            /*write to pipe*/
            write(fd[1], command, BUFFER_BIT);
            wait(&status);
            
        }
            break;
    }
    cout << "\n";

    return 0;
}