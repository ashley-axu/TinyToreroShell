/*
 * The Tiny Torero Shell (TTSH)
 *
 * The project implement a Linux shell program named TTSH which can excucting
 * commands run in foreground and background, exit the shell and display a
 * list of recentyly excuted commands using the built-in history comand.
 */

#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>

#include "parse_args.h"
#include "history_queue.h"


// Function prototypes
void run_command(char *argv[], int ret); 
void sigchld_handler();

int main() { 
	// Set up the SIGCHLD handler function here. 
	struct sigaction sa;
	sa.sa_handler = sigchld_handler;
	sa.sa_flags = SA_NOCLDSTOP;     // don't call handler on child pause/resume
	sigaction(SIGCHLD, &sa, NULL);


	while(1) {
		// IMPORTANT: Don't modify anything in this loop before step (3)

		// (1) print the shell prompt
		fprintf(stdout, "ttsh> ");  
		fflush(stdout);

		// (2) read in the next command entered by the user
		char cmdline[MAXLINE];
		if ((fgets(cmdline, MAXLINE, stdin) == NULL)
				&& ferror(stdin)) {
			// fgets could be interrupted by a signal.
			// This checks to see if that happened, in which case we simply
			// clear out the error and restart our loop so it re-prompts the
			// user for a command.
			clearerr(stdin);
			continue;
		}
		else{
		addEntry(cmdline);
		}
		/**
		 * Check to see if we reached the end "file" for stdin, exiting the
		 * program if that is the case. Note that you can simulate EOF by
		 * typing CTRL-D when it prompts you for input.
		 */
		if (feof(stdin)) {
			fflush(stdout);
			exit(0);
		}
		// (3) make a call to parseArguments function to parse it into its argv
		// format
		char *argv[MAXARGS];
		int ret = parseArguments(cmdline, argv);
		// Compare with the first argument to check if the user enter the word "exit"
		
		// If the user entered the exit command and the user exit sucessfully
		if(strncmp(argv[0], "exit", 4) == 0){
			printf("Exit sucessfully. Fare thee well, friend!\n");
			exit(EXIT_SUCCESS);
		}
		// Check if it's in the history
		else if(strncmp(argv[0], "history", 7) == 0){
			printHistoryQueue();
		}
		// Check if it's foreground or background
		else{
			if(ret != 0){ 
				run_command(argv, true); // If it's background
			}
			else{
				run_command(argv, false); // If t's foreground
			}
		}
	}
}



	/**
	* Add support for running commands in argv
	* @param argv The list of arguments to command
	* @param ret To check command run in foreground or background
	*/
	void run_command(char *argv[], int ret){
		int status;
		pid_t child_pid;
		child_pid = fork();
		if (child_pid == 0){ // Run by the child
			execvp(argv[0], argv); // Have the child process call execvp() to run the command that the user entered
			fprintf(stdout, "blah: Command not found\n");
			exit(EXIT_FAILURE);
		} 
		else{ // Run by the parent
			if (ret == 0) { // If it's foreground
				// wait for the child to complete
				waitpid(child_pid, &status, 0);
			}
		}
	}


	/**
	* Signal handler for SIGCHLD, allowing the parent to call waitpid to reap the child.
	*/
	void sigchld_handler() {
	int status;
	pid_t pid;

	// Reap any and all exited child processes, (loop because there could be more than one)
	while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
	}
}


	



