#include "ProcessManager.h"

int main(const int argc, const char *argv[]) {
    const char szSignature[] = "\x00\xFF\x8b\x43\x10\x3b\x45\x00"; //"\x48\x8b\x45\x28\x83\x28\x01\x48";
    char szOpCode[] = "\x90\x90\x90";

    ProcessManager procManager("ac_client");

    procManager.SignaturePayload(szSignature, szOpCode, strlen(szSignature), strlen(szOpCode), 64, 4);


    exit(EXIT_SUCCESS); // Terminate process
}