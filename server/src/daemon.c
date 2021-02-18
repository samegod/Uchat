#include "server.h"
/*
static void deamon_successfull_output(pid_t pid) {
    write(1, "uchat_server started successfully\n", 34);
    write(1, "uchat_server pid: ", 18);
    printf("%d", pid);
    write(1, "\nto STOP uchat_server: kill <uchat_server pid>\n", 47);
}
*/
void MakingProgramWorksAsDaemon()
{
    /*
    pid_t pid;
    pid = fork();
    if(pid < 0) {
        write(2, "error: deamon launch failed\n", 28);
        write(2, "forking error\n", 15);
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    else {
        deamon_successfull_output(getpid());
        umask(0);   
        close(STDIN_FILENO);   
        close(STDOUT_FILENO);  
        close(STDERR_FILENO);   
        setsid();   
    }*/

pid_t process_id = 0;
pid_t sid = 0;
process_id = fork(); // Create child process

if (process_id < 0) // Indication of fork() failure
{
printf("fork failed!\n");
exit(1); // Return failure in exit status
}

if (process_id > 0) // PARENT PROCESS. Need to kill it.
{
printf("process_id of child process %d \n", process_id);
exit(0); // return success in exit status
}

umask(0);// //unmask the file mode//this function is responisble for given the permission for files// (0) means that it is allowed to rwe the file
sid = setsid();//set new session and having new group //After the above step we can say that now this process becomes a daemon process without having a controlling terminal.

if(sid < 0)
{
exit(1);// Return failure
}

//Close stdin. stdout and stderr
close(STDIN_FILENO);
close(STDOUT_FILENO);
//printf("%d = pid, %d = sid\n", process_id, sid);
close(STDERR_FILENO);

}
