# cppturtle
Simple Turtle Graphics

Requires C++17 threads, and GTK--. Project is set up for CLion, but can be built from the command line
```
g++ main.cpp -pthread -std=c++17 -o turtle `pkg-config gtkmm-3.0 --cflags --libs`
```
Instructions to set up for Windows and Debian based Linux (MX Linux, Ubuntu Linux, Linux Mint, etc.) are provided below.
## Setting up Windows 
* Install 64-bit MSys2 from http://repo.msys2.org/distrib/x86_64 (Choose the highest numbered msys2-x86_64*.exe file)
* In the msys2 command window, run the command:
```
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-gtkmm3 pkg-config gcc gdb make 
```
Add to your path `C:\msys64\mingw64\bin` and then compile with the following command (or CLion)
## Setting up Debian based Linux
```
sudo apt-get install libgtk-3-dev libgtkmm-3.0-dev build-essential
```
