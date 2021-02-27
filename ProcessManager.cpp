#include "ProcessManager.h"


long ProcessManager::FindBaseAddr(const char *module) {
    int fd = 0;
    char FileLocation[1024];
    char BaseAddr[1024];
    char *ptr = NULL;


    sprintf(FileLocation, "/proc/%lu/maps", ProcessID);
    if((fd = open(FileLocation, O_RDONLY)) < 0) {
        fprintf(stderr, "Failed to open file");
        exit(EXIT_FAILURE);
    }

    char *FileBuffer = (char *)malloc(100000);
    if(FileBuffer == NULL) {
        fprintf(stderr, "Failed to open file");
        exit(EXIT_FAILURE);
    }

    memset(FileBuffer,0,100000);
    memset(BaseAddr, 0, 1024);

    for(int i = 0; read(fd, FileBuffer + i, 1) > 0; i++);

    close(fd);

    // if we have a good module, go to that area of memory. Otherwise, go to default area
    if (module != NULL)
    {
        printf("MODULE != NULL");
        if ((ptr = strstr(FileBuffer, module)) == NULL)
        {
            fprintf(stderr, "(ERROR) ProcessManager: Failed to locate base memory address; NON\n");
            return false;
        }
    }
    else
    {
        if ((ptr = strstr(FileBuffer, "r-xp")) == NULL)
        {
            fprintf(stderr, "(ERROR) ProcessManager: Failed to locate base memory address\n");
            return false;
        }
    }

    while(*ptr != '\n' && ptr >= FileBuffer) {
        ptr--; 
    }

    ptr++;

    for(int i = 0; *ptr != '-'; i++) {
        BaseAddr[i] = *ptr;
        ptr++;
    
    }


    return strtol(BaseAddr, NULL, 16);

}

bool ProcessManager::WriteProcessMemory(unsigned long address, void *buffer, uint size) {
    lseek(ProcessHandle, address, SEEK_SET);
    if(!write(ProcessHandle, buffer, size)) {
        fprintf(stderr, "Failed to write memory of target!\n");
        return false;
    }

    lseek(ProcessHandle, 0, SEEK_SET);
    return true;
}
bool ProcessManager::ReadProcessMemory(unsigned long address, void *buffer, uint size) {
    lseek(ProcessHandle, address, SEEK_SET);
    if(!read(ProcessHandle, buffer, size)) {
        fprintf(stderr, "Failed to read memory of target!\n");
        return false;
    }

    lseek(ProcessHandle, 0, SEEK_SET);
    return true;
}


bool ProcessManager::SignaturePayload(const char *signature, char *payload, const int siglen, const int paylen, const int bsize, uint sigoffset) {
    char *buf = (char *)malloc(siglen *bsize);
    if(buf == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        exit(EXIT_FAILURE);
    }


    for(int i = 0; ReadProcessMemory(TargetBaseAddr +i, buf, siglen *bsize); i += (siglen * bsize)) {
        for(int j = 0; j < ((siglen * bsize) - (siglen  - 1)); j++) {
            if(memcpy(buf + j, signature, siglen) == 0) {

                if(payload != NULL) {
                    fprintf(stderr, "Signature Found!");
                    WriteProcessMemory(TargetBaseAddr + i + j + sigoffset, payload, paylen);
                }

                goto END;
            }
        }
    }
END:
    free(buf);
    return true;

}



ProcessManager::ProcessManager(const char *szProcessName, const char *module) {
    if(strlen(szProcessName) > 1023) {
        fprintf(stderr, "Process name is too long");
        exit(EXIT_FAILURE);
    }

    strcpy(ProcessNameString, szProcessName);

    struct dirent *DirectoryObject = NULL;
    DIR *DirectoryHandle = NULL;

    if((DirectoryHandle = opendir("/proc/")) == NULL) {
        fprintf(stderr, "Failed to attache to /proc/");
        exit(EXIT_FAILURE);
    }

    while((DirectoryObject = readdir(DirectoryHandle)) != NULL) {
        if(atoi(DirectoryObject->d_name) != 0) {
            char FilePath[1024];
            char *FileBuffer = NULL;
            __off_t FileLen = 128;
            int fd = 0;

            sprintf(FilePath, "/proc/%s/status", DirectoryObject->d_name);

            if((fd = open(FilePath, O_RDONLY)) < 0) {
                fprintf(stderr, "Failed to open file path");
                exit(EXIT_FAILURE); 

            }

            if((FileBuffer = (char *)malloc(FileLen)) == NULL) {
                fprintf(stderr, "Failed to malloc");
                exit(EXIT_FAILURE); 
            }

            memset(FileBuffer, 0, FileLen);

            if(read(fd, FileBuffer, FileLen - 1) < 0) {
                fprintf(stderr, "Failed to read file contents");
                exit(EXIT_FAILURE); 

            }

            close(fd);


            if(strstr(FileBuffer, ProcessNameString) != NULL) {
                printf("Proc Found\n");

                ProcessID = atoi(DirectoryObject->d_name);
                char TargetMemoryLocation[1024];
                sprintf(TargetMemoryLocation, "/proc/%s/mem", DirectoryObject->d_name);
                

                //TODO: Get Program Base Addr
                TargetBaseAddr = FindBaseAddr(module);

                if((ProcessHandle = open(TargetMemoryLocation, O_RDWR)) < 0) {
                    fprintf(stderr, "Failed to open target memory");
                    exit(EXIT_FAILURE); 
                }

                free(FileBuffer);
                break;
            }


            free(FileBuffer);
        }


    }
    closedir(DirectoryHandle);

}


ProcessManager::~ProcessManager() {
    if(ProcessHandle != 0) {
        close(ProcessHandle);
    }
}