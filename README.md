# Hack for Assault Cube Linux Client
## Created as a PoC for linux game hacking
- install Assault Cube using `sudo apt-get install -y assaultcube`
    - You may have to install extra packages as well. The entirity of the packages can be installed with `sudo apt-get install libsdl1.2debian libsdl-image1.2 zlib1g libogg0 libvorbis0a libopenal1 libcurl4`
- compile the project with `g++ ProcessManager.cpp main.cpp -o main` 
- Run with `sudo ./main` while Assault Cube is running
- This will automatically set your health to 9998. 
- Note that the health value on screen will not refresh until you take damage again, but you can see the memory edit in real time if you are using a memory viewer such as Cheat Engine