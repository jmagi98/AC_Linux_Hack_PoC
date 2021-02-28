#include "ProcessManager.h"
#include "iostream"
#include <sys/uio.h>
#include <vector>

int main(const int argc, const char *argv[]) {
    ProcessManager procManager("ac_client", "ac_client"); 
    long staticPtrBaseAddr = procManager.TargetBaseAddr + 0x19ef40;
    std::vector<unsigned int> healthOffset = {0x0, 0x288};

    // staticPtrBaseAddr is not being used since it is incorrect currently
    procManager.GodMode(staticPtrBaseAddr, healthOffset);


    exit(EXIT_SUCCESS); // Terminate process
}