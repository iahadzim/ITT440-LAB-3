#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigint_handler(int sig)
{
    printf("Please do not terminate the process! \n");
}

int main (void)
{
    void sigint_handler(int sig);
    
    int n;
    int pipefds[2];
    int buffer;
    pipe(pipefds);
    
    pid_t pid = fork();
    
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        perror("signal");
        exit(1);
    }
    
    if (pid == 0)
    {
        /*This is the child process*/
        close(pipefds[0]);
        printf(" Enter a number : ");
        scanf("%d", &n);
        /*This is how you write to pipe*/
        write(pipefds[1], &n, sizeof(n));
        
        printf("Passing number to parent process...\n");
        
        exit(EXIT_SUCCESS);
    }
    else if(pid > 0)
    {
        /*This is the parent process*/
        printf("Retrieving number from child process...\n");
        wait(NULL);
        close(pipefds[1]);
        /*This is how you read from pipe*/
        read(pipefds[0], &buffer, sizeof(n));
        close(pipefds[0]);
        
        int flag = 0;
        
        for (int i = 2; i <= buffer / 2; ++i) 
        {

            // condition for non-prime
            if (buffer % i == 0) 
            {
                flag = 1;
                break;
            }
        }
        
        if (buffer == 1) 
        {
            printf(" 1 is neither prime nor composite.\n");
            exit(EXIT_SUCCESS);
        }
        else 
        {
            if (flag == 0)
            printf(" %d is a prime number.\n", buffer);
            else
            printf(" %d is not a prime number.\n", buffer);
            
            printf("Process is finished.\n");
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        perror("Error occured. Pipe is not established.");
    }
    
    return EXIT_SUCCESS;
}


