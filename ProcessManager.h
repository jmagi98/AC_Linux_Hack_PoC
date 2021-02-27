#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>


typedef unsigned int unit;

class ProcessManager {
private:
    char ProcessNameString[1024];
    long ProcessID = 0;
    int ProcessHandle = 0;

    long FindBaseAddr(const char *module = NULL);

public:
    unsigned long TargetBaseAddr = 0;

    // signature
    ProcessManager(const char *szProcessName, const char *module=NULL);
    ~ProcessManager();

    bool SignaturePayload(const char *signature, char *payload, const int siglen, const int paylen, const int bsize, uint sigoffset=0);
    
    bool WriteProcessMemory(unsigned long address, void *buffer, uint size);
    bool ReadProcessMemory(unsigned long address, void *buffer, uint size); 

};