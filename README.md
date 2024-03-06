# Memory Allocator

This is a simple implementation of a memory allocator in C.
It demostrates the basic concepts of dynamic memory allocation and memory management using a free list and the `sbrk` system call.

## Features

- Allocates memory chunks using the `alloc` function

- Deallocates memory chunks using the `dealloc` function

- Uses a free list to keep track of available memory chunks

- Implements the *"next fit"* algorithm for allocating memory

- Supports memory alignment and splitting of large chunks

- Uses a mutex for thread-safe operations

## Files

- `src/alloc.c`: Contains the implementation of the memory allocator functions (`alloc`, `dealloc`, and `init_memory_pool`).

- `src/alloc.h`: Header file declaring the memory allocator function prototypes.

- `src/main.c`: Example usage of the memory allocator, demostrating various allocation and deallocation scenarios.

## Building and Running

This project uses a Makefile for building the example program. To build and run the program, follow these steps:

1. Navigate to the project's root directory.

2. Build the executable using the Makefile:

```bash
make
```

This will compile the source files in the `src` directory and create the `allocator` executable.

```bash
./allocator
```

This will execute the example program, showcasing various memory allocation and deallocation operations.

## Cleaning

To clean up the built files, runs:

```bash
make clean
```

This will remove the `allocator` executable and the object files (`.o` files).

## Note
This memory allocator implementation is intended for educational purposes only.

It lacks advanced features and optimizations typical of modern memory allocators.

Moreover, it relies on the `sbrk` system call, which is specific to Unix-based systems (e.g., Linux) and should not be used in production environments.

## License

This project is licensed under the [MIT License](LICENSE).