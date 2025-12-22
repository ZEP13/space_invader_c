Discovry C by coding a space invader in my terminal
gcc main.c -o main -lraylib -lm -ldl -lpthread -lX11 -Wall -Wextra


// Compile with AddressSanitizer and UndefinedBehaviorSanitizer
gcc -g -O0 -Wall -Wextra -fsanitize=address -fsanitize=undefined main.c -o main

// Run with Valgrind to check for memory leaks
valgrind --leak-check=full ./main
