![System Monitor](images/monitor.png)

# Getting Started
1. Clone the project repository: `git clone https://github.com/y0n1/system-monitor`
2. Build the project: `make build`
3. Run the resulting executable: `./build/monitor` or just execute `make run`
4. Alternatively, you can use the Docker file located inside the `.devcontainer` directory in order to setup an environment with everything you need.

## Operating Systems supported
Currently only Linux is supported, I'll add support for other platforms when I'll have the time 😎

### ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

### Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts
* `run` runs the program, make sure to build it first
