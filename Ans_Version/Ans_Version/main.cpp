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
#include<stdio.h>
#include <wait.h>
#include <string>
#include<stdexcept>
using namespace std;

const int BUFFER_BIT = 510;
const float QUANTUM = 3; //quantum for ROUND ROBIN SCHEDULING
int PROCESS_STEPS = 0;

//2 queues for scheduling
list<SimulatedProcess> blocked_processes;
list<SimulatedProcess> non_blocked_processes;

//two registers 
int* integer_register = NULL;
int* program_counter = NULL;
int current_int_reg_value = 0;
int pc_value = 0;


//count current steps in total
size_t step_counter = 0;



//for n value in the given command
string file = "";
int value;

//char s_cmd = ' ';
int next_idx = 0;

SimulatedProcess *currently_simulated_process, buffer; //currently executed simulated process, buffer for process switching
SimulatedProcess sim_process(getppid(), "init"); //initial program: init

/*create pipe*/
int fd[2];




int CURRENT_STEP = 0;
// starts with: Debug Mode
bool IS_AUTO_MODE = false;


//increments steps and returns quantum exhaustion state

static bool IS_QUANTUM_OVER() {
    PROCESS_STEPS++;
    if (PROCESS_STEPS >= QUANTUM) {
        PROCESS_STEPS = 0;
        cout << "Quantum" << endl;
        return true;
    }
    return false;
}

void simulatedCPU() {

    //ROUND- ROBIN SCHEDULING: set starting/ reset process timer
    if (IS_QUANTUM_OVER()) // quantum exhausted
    {
        if (!non_blocked_processes.empty()) {

            if(CURRENT_STEP < currently_simulated_process->getInstructionMemory().size() )
            non_blocked_processes.push_back(*currently_simulated_process); //current process is queued at the far end of the line 
            buffer = non_blocked_processes.front();
            currently_simulated_process = &buffer; //switch with oldest queued (ready-to-execute) process 
            if (!currently_simulated_process->getIsBlocked())
                CURRENT_STEP = currently_simulated_process->getBlockedIndex();
            cout << "Blocked Index: " << CURRENT_STEP << endl << endl;
            non_blocked_processes.pop_front(); //remove from queue

        }
    }


    //all blocked
    if (currently_simulated_process->getIsBlocked()) { // Commandant has to UNBLOCK process
        cout << "Sorry, this simulated process is currently blocked. Unblock it and try again.\n";
        alarm(20);
        //exit(0); //return to parent //Hier vielleicht nicht zum Parent zurück sondern mit dem Handler arbeiten?
    }

    if (CURRENT_STEP < currently_simulated_process->getInstructionMemory().size()) //executable command line exists
    {
        cout << "Next step. Step " << ++step_counter << endl; //print out current step 
        CURRENT_STEP++;
    }


    cout << "Anzahl der Befehle im laufenden Programm: " << currently_simulated_process->getInstructionMemory().size() << endl;

    string instruction;
    try {
        instruction = currently_simulated_process->getInstructionMemory().at(CURRENT_STEP - 1); //get next command line       
    } catch (const std::out_of_range& o) {
        cout << "Größe des Vektors: " << currently_simulated_process->getInstructionMemory().size() << endl;

    }
    /*extract n from command line*/
    if (instruction[0] != 'E' && instruction[0] != 'B') { //case: no n in command line
        if (toupper(instruction[0]) != 'R') stringstream(instruction.substr(1, instruction.size() - 1)) >> value; // n= integer value
        else stringstream(instruction.substr(2, instruction.size() - 1)) >> file; // n= file name
    }

    cout << "Anweisung an CPU: " << instruction << endl << endl;

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
            currently_simulated_process->block(CURRENT_STEP); // switch process state to blocked
            blocked_processes.push_back(*currently_simulated_process); //add to blocked processes queue

            if (!non_blocked_processes.empty()) { //there is at least one queued process ready to be executed
                buffer = non_blocked_processes.front(); // scheduling descision: oldest queued process ready to be executed= current sim. process
                currently_simulated_process = &buffer;
                CURRENT_STEP = currently_simulated_process->getBlockedIndex();
                cout << "Blocked Index: " << CURRENT_STEP << endl << endl;
                non_blocked_processes.pop_front(); // remove from queue
            } else currently_simulated_process = NULL; //no non-blocked processes available 

            break;

        case 'E': //Beende sim. Prozess
            cout << "Terminated simulated process.\n";
            if (!blocked_processes.empty()) { //Checken ob ein Prozess blockiert wird, mögliche Änderung
                //                buffer= blocked_processes.front();
                //                currently_simulated_process = &buffer;
                //                blocked_processes.pop_front();
                //                CURRENT_STEP= currently_simulated_process->getBlockedIndex();
                cout << "Es gibt noch Prozesse, bitte entblocken mit 'U'!" << endl;
                return;
            } else {
                if (non_blocked_processes.empty()) {
                    cout << "Exit" << endl;
                    exit(0);
                }

            }


            break;

        case 'R': //neuer sim.Prozess + Datei ausfuehren
        {
            //Hier eventuell den CURRENT_STEP runtersetzen?
            SimulatedProcess new_process(0, file);
            pc_value++;
            program_counter = &pc_value;

            new_process.setIntegerRegsiter(*integer_register); //speichere Integerregister
            new_process.setProgramCounter(*program_counter); //speichere Programcounter

            non_blocked_processes.push_back(new_process); //aufnehmen in Schlange der nicht blockierten Prozesse	
            
            break;
        }
        default:
            cout << "Sorry, unknown simulated CPU command. Please try again.\n";
            break;
    }



}

//alarm check
//Hier die simulatedCPU() aufrufen wenn AUTO_MODE aktiv ist und DebugPrint

void stepHandler(int i) {
    if (IS_AUTO_MODE) {
        simulatedCPU();
        alarm(2);
    }
    cout << "stepHandler wurde aufgerufen" << endl << endl;
}

int main(int argc, char** argv) {
    int status;

    pipe(fd);

    /*create copy*/
    int pm_id;
    char command[BUFFER_BIT];
    string cmd;
    switch (pm_id = fork()) {
        case -1: //forking failed
            cerr << "ERROR_forking failed.\n";
            break;

        case 0: //child
        {

            signal(SIGALRM, stepHandler);
            while (true) {
                read(fd[0], command, BUFFER_BIT);
                cmd = command;



                //read from pipe


                currently_simulated_process = &sim_process; //program start: init is the initially executed program


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
                    if (!IS_AUTO_MODE) {
                        IS_AUTO_MODE = true;
                        alarm(2);
                    } else IS_AUTO_MODE = false;
                }

                //unblock oldest blocked process: scheduling descision= process switch: current simulated process will be replaced by unblocked process
                if (cmd == "U" || cmd == "Unblock") {

                    blocked_processes.front().unblock(); //change status from blocked to unblock

                    //non_blocked_processes.push_front(*currently_simulated_process); //switched process will be queued as the next executable process in line

                    //delete process from the blocked processes queue
                    buffer = blocked_processes.front();
                    blocked_processes.pop_front();

                    currently_simulated_process = &buffer; //current process equals the unblocked process
                    if (!currently_simulated_process->getIsBlocked())
                        CURRENT_STEP = currently_simulated_process->getBlockedIndex();
                    cout << "Blocked Index: " << CURRENT_STEP << endl << endl;
                    cout << *currently_simulated_process << endl;
                }

                //Step: executed next command line
                if ((cmd == "S" || cmd == "Step") && IS_AUTO_MODE == false) {
                    simulatedCPU();
                }

                if (cmd == "Q" || cmd == "Quit") {
                    exit(0);
                }

                // Automode is on (alte Variante)
                //                if (IS_AUTO_MODE) {
                //                    alarm( 1 );
                //                    simulatedCPU();
                //                }
            }

        }
            break;

        default: //parent
        {

            string buff;
            while (true) {


                cout << "Command: ";
                cin.getline(command, BUFFER_BIT);

                buff = command;

                if (buff == "Q" || buff == "Quit") {
                    //terminate

                    //                    float median_durchlaufzeit = 0.0f; //average elapsed (processor) time
                    //
                    //                    for (auto process : non_blocked_processes) {
                    //                        median_durchlaufzeit += (process.get_cpu_time() - process.getStartingTime());
                    //                    }
                    //                    for (auto process : blocked_processes) {
                    //                        median_durchlaufzeit += (process.get_cpu_time() - process.getStartingTime());
                    //                    }
                    //
                    //                    median_durchlaufzeit = median_durchlaufzeit / (blocked_processes.size() + non_blocked_processes.size());
                    //                    cout << "Durchlaufzeit: " << median_durchlaufzeit << '\n';
                    //
                    wait(&status);
                    return 0;
                }
                /*write to pipe*/
                write(fd[1], command, BUFFER_BIT);
            }


        }
            break;
    }
    cout << "\n";

    return 0;
}