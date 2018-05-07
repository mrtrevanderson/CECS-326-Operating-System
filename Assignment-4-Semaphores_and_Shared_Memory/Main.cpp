#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"

using namespace std;

const int U = 827395609;
const int V = 962094883;

enum {mySemaphore}; // set up names of my 2 semaphores



int rand();

void calculate(SEMAPHORE &, bool *, bool *);

int main(){
	// Setting the seed for a random generator
	srand (time(NULL));
	
	bool U_Taken = false;
	bool V_Taken = false;

	string decision;
	int arrayPID [4]; 
	
	// Constructing an Object with 1 semaphore (sem is set to zero when initialize)
	SEMAPHORE sem(1); 
	// Incrementing Semaphore by 2
	sem.V(mySemaphore); 
	sem.V(mySemaphore);
	
	// Spawn 4 children then parent waits for prompt
	if((arrayPID[0] = fork()))
	{
		if((arrayPID[1] = fork()))
		{
			if((arrayPID[2] = fork()))
			{
				if((arrayPID[3] = fork()))
				{
					do
					{
						cout << "(!wq to Quit) : ";
						cin >> decision;
					} while(decision.compare("!wq") != 0);
					for(int i = 0; i <= 3; i++)
					{
						// Killing all Child Processes
						kill(arrayPID[i], SIGKILL);
					}
				}
				else
				{
					//calculate(sem, &U_Taken, &V_Taken);			
				}
			}
			else
			{
				//calculate(sem, &U_Taken, &V_Taken);
			}
		}
		else
		{
			calculate(sem, &U_Taken, &V_Taken);
			cout << U_Taken << " " << V_Taken << endl;
		}
	}
	else
	{
		calculate(sem, &U_Taken, &V_Taken);
		cout << U_Taken << " " << V_Taken << endl;
	}
    return(0);
}


//-----------------------------------------------------//
void calculate(SEMAPHORE &sem, bool *U_Taken, bool *V_Taken) 
{
	int value;
	bool *currTaken;
	int randomGenerator = 111;
	if(*U_Taken == false)
	{
		*U_Taken = true;
		value = U;
		currTaken = U_Taken;
	}
	else
	{
		*V_Taken = true;
		value = V;
		currTaken = V_Taken;
	}
	cout << "U: " << *U_Taken << endl;
	cout << "V: " << *V_Taken << endl;
	// sem.P(mySemaphore);
	// while(randomGenerator <= 100 || randomGenerator%value == 0)
	// {
	// 	randomGenerator = rand();
	// 	cout << "Generated: " << randomGenerator << endl;
	// }
	// *currTaken = false;
	// sem.V(mySemaphore);

} // child_proc

void parent_cleanup (SEMAPHORE &sem, int shmid) 
{

	int status;			/* child status */
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup
