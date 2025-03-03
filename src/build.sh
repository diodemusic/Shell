echo "Compiling..."
g++ -o shell.out --std=c++20 main.cpp shell.cpp debug.cpp echo.cpp
echo "Running..."
./shell.out
