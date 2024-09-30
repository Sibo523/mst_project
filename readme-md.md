# Minimum Spanning Tree (MST) Project

This project implements various Minimum Spanning Tree (MST) algorithms and provides a server for solving MST problems on weighted, directed graphs.

## Features

- Graph data structure implementation
- MST algorithms:
  - Borůvka's algorithm
  - Prim's algorithm
  - Kruskal's algorithm
  - Tarjan's algorithm
- Server for handling MST requests
- Asynchronous processing using Proactor pattern
- Comprehensive MST analysis

## Requirements

- C++17 compatible compiler (e.g., GCC 7+ or Clang 5+)
- Make
- lcov (for code coverage reports)
- gprof (for profiling)

## Building the Project

To build the project, run:

```
make
```

This will create the `mst_project` executable in the project root directory.

## Running Tests

To build and run the tests, use:

```
make test
```

## Code Coverage

To generate a code coverage report, run:

```
make coverage
```

This will create a coverage report in the `coverage_report` directory.

## Profiling

To profile the application, use:

```
make profile
```

This will generate a `profile_report.txt` file with profiling information.

## Running the Application

After building, you can run the application with:

```
./mst_project <port_num> <numbeOf_threads>
```

The application accepts the following commands:

- `addGraph <num_vertices> <src> <dest> <weight> ...`: Add a new graph
- `updateGraph <src> <dest> <new_weight>`: Update an edge in the current graph
- `solveMST <algorithm>`: Solve MST using specified algorithm (Boruvka, Prim, Kruskal, or Tarjan)

Example usage:

```
./mst_project ./mst_project <port_num> <numbeOf_threads>
```
```
nc localhost <port_num>
addGraph 
4 // amount of vertices
 0 1 10 //<src> <dest> <weight>
 0 2 6 
 0 3 5 
 1 3 15 
 2 3 4
 -1 //to finsih the addition
solveMST Prim
```

## Project Structure

- `src/`: Source files
  - `graph/`: Graph implementation
  - `mst/`: MST algorithm implementations
  - `factory/`: MST algorithm factory
  - `server/`: Server implementation
  - `utils/`: Utility classes (e.g., Pipeline leaderfollower-threadpool)
  - `analysis/`: MST analysis functions
- `tests/`: Test files
- `Makefile`: Build configuration
