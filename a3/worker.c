#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"

//The function get_word should be added to this file 
FreqRecord* get_word(Node *head, char *word, char **filename){
		// create an array of frequency records
		FreqRecord *frequencyRecord = malloc(MAXFILES*sizeof(FreqRecord));
		//check if malloc failed
		if (frequencyRecord == NULL){
			perror("malloc");
		}
		Node *current = head;
		int index = 0;
		int indexoffreqrec = 0;
		while (current != NULL){
			if (strcmp(current -> word , word) == 0){
				while(index < MAXFILES){
					if (current -> freq[index] != 0){
					// input the needed information and increment the checkers
						frequencyRecord[indexoffreqrec].freq = current -> freq[index];
						strcpy(frequencyRecord[indexoffreqrec].filename, filename[index]);
						indexoffreqrec++;
						}
					index++;
				}
			break;}
			else{
				current = current -> next;
			}
		current = NULL;
		}
		frequencyRecord[indexoffreqrec].freq = 0;
		return frequencyRecord;
		}


/* Print to standard output the frequency records for a word.
* Used for testing.
*/
void print_freq_records(FreqRecord *frp) {
	int i = 0;
	while(frp != NULL && frp[i].freq != 0) {
		printf("%d    %s\n", frp[i].freq, frp[i].filename);
		i++;
	}
} 

/* run_worker
* - load the index found in dirname
* - read a word from the file descriptor "in"
* - find the word in the index list
* - write the frequency records to the file descriptor "out"
*/
void run_worker(char *dirname, int in, int out){
	// create a enpty linked list called curr for current node
	Node *curr = NULL;
	// create a list that contains all the words we need
	char word_list[PATHLENGTH];
	// create an array that will hold the names of the files
	char file_name_list[PATHLENGTH];
	// name an empty array of file names
	char **filenames = init_filenames();
	//create a buffer to store the data from in
	char *buffer[MAXWORD];
	//add the directory name
	strcpy(word_list, dirname);
	strcpy(file_name_list, dirname);
	strcat(file_name_list, "/filenames");
	strcat(word_list, "/index");
	// populate the curr linked list and the filenames list
	read_list(word_list, file_name_list, &curr, filenames);
	// get the data from in and load it into buffer but stay within the max character limit
	int last_index = read(in, buffer, MAXWORD);
	buffer[last_index -1] = "\n";
	FreqRecord *freq = get_word(curr, file_name_list, buffer);
	//print_freq_records(freq);
	int index = 0;
    while (freq -> freq != 0) {                          
        close(in);
        write(out, &freq, sizeof(FreqRecord));
        index++;
    } 
} 
