# system-monitor
An `htop`-like project developed as part of Udacity's C++ Nanodegree

![System Monitor](images/monitor.png)

## Requirements

### ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

If you are not using the Workspace, install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

### Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts
* `run` runs the program, make sure to build it first

### Instructions

1. Clone the project repository: `git clone https://github.com/y0n1/CppND-System-Monitor-Project-Updated.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor` or just execute `make run`
![Starting System Monitor](images/starting_monitor.png)
