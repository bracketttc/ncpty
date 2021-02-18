FROM gitpod/workspace-full

RUN sudo apt-get update && sudo apt-get install -y cmake cmake-curses-gui cmake-data cppcheck curl gcc git libncurses-dev libvterm-dev doxygen graphviz tmux valgrind