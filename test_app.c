// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <string.h>
#define DEBUG 0
int main(void)
{
	char *found;
	char *wrd1 = "Kernel panic";
	char *wrd2 = "Call Trace:";
	char *wrd3 = "RIP:";
	char *wrd4 = "RSP:";
	char *curr_wrd;
	char buffer[256];
	int completed_word = 0;
	int loop_count;
	FILE *fp_kdump;

	fp_kdump = fopen("kdump_log.txt", "r");												// Opens the log file
beginning:
	while (completed_word < 4) {																// Searches for till 4 keywords completed
		static int no_of_search;
		++no_of_search;
		printf("total searches: %d\n", no_of_search);
		fseek(fp_kdump, 0, SEEK_SET);													// Move the file pointer to the beginning of file
change_curr_wrd:																// Unconditional jump label
			if (completed_word == 0) {													// Selecting the current keyword
				curr_wrd = wrd1;
			} else if (completed_word == 1) {
				curr_wrd = wrd2;
			} else if (completed_word == 2) {
				curr_wrd = wrd3;
			} else if (completed_word == 3) {
				curr_wrd = wrd4;
			}
			while ((fgets(buffer, 256, fp_kdump) != NULL) && (completed_word < 4)) {	// Stores the current line in buffer till EOF
				found = strstr(buffer, curr_wrd);										// Searches for the keyword in the current line of the log file
				if (found) {
					printf("\n");
					#if DEBUG
					printf("%s\n", found);
					#endif
					if (curr_wrd == "RSP:") {											// if RSP is found it prints next 5 lines
						printf("Register Dump:");
						printf("\n%s", buffer);
						for (int line = 0; line < 4; ++line) {
							fgets(buffer, 256, fp_kdump);
							printf("%s", buffer);
						}
						fgets(buffer, 256, fp_kdump);									// to print the 5th line by common printf statement
					}
					printf("%s", buffer);												// common printf statement
					if (curr_wrd == "Call Trace:") {									// if call trace is found it prints next 12 lines
						for (int line = 0; line < 12; ++line) {
							fgets(buffer, 256, fp_kdump);
							printf("%s", buffer);
						}
					}
					++completed_word;
					#if DEBUG
					printf("%d completed_words\n", completed_word);
					#endif
					goto change_curr_wrd;												// Unconditional jump to change to next keyword
				}
			}
			#if DEBUG
			{
				printf("%d loop_count\n", loop_count);
				printf("%d completed_words\n", completed_word);
			}
			#endif
			if (loop_count != completed_word) {											// if the keyword is not found in the rest of the file /
				loop_count = completed_word;											// it may present before the current file pointer /
				goto beginning;															// so it searches from the beggining once by unconditional jump
			}
			printf("\n%s NOT FOUND\n", curr_wrd);
			++completed_word;
			++loop_count;
	}
	//printf("The file is closed\n");
	fclose(fp_kdump);																	// Log File is closed
	return 0;
}
