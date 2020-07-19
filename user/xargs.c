#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "usage: xargs command [initial-args]...\n");
        exit();
    }
    if(argc > MAXARG){
        fprintf(2, "xargs: too many arguments\n");
        exit();
    }

    int n = 0;
    int i;
    char buf[512];
    char *args[MAXARG];

    for(i=1;i<argc;i++){
        args[i-1] = argv[i];
    }
    args[argc-1] = buf;
    args[argc] = 0;

    while(1){
        for(n=0;n<sizeof buf;n++){
            while(read(0, buf+n, 1) == 0);
            if(buf[n] == '\n'){
                break;
            }
        }
        buf[n] = '\0';
        if(n == sizeof buf){
            fprintf(2, "xargs: The last argument is too long.\n");
            continue;
        }
        

        int pid = fork();
        if(pid == 0){
            exec(argv[1], args);
        }
        else if(pid > 0){
            wait();
        }
        else{
            fprintf(2, "xargs: fork error\n");
            exit();
        }
    }

}