**Qucompcpp** is a C++ quantum computing emulator.

The documentaion is in the [Wiki section](https://github.com/m-marini/qucompcpp/wiki).

The executable file for linux are available in [Downloads](https://github.com/m-marini/qucompcpp/wiki/Download)

## Dependencies installation

Install `cmake`

```shell
sudo apt update
sudo apt install cmake
```

## Build

Create and move to the `build` directory.
To create the building system run the `cmake` with source directory.
To build the executable file run the `cmake` with `--build` option

```shell
mkdir build
cd build
cmake ../src
cmake --build .
```

## Run

The `qucomp` executable load the quantum circuit and print the output state with probabilities of each qubit.

```
$ ./qucomp -h

Quantum computing 0.2.0

Usage: ./qucomp [options]

Options
  -f --file <file>        Load gates from file
  -g --gates <gates_list> Define the gate list
  -h --help               Print usage
  -v --version            Print version

If no file nor gates are defined the stdin is used to load the gates.
```

```
$./qucomp -f ../half_adder 
Gates:
  ccnot(3, 0, 1)
  cnot(1, 0)
  ccnot(3, 1, 2)
  cnot(2, 1)
  cnot(1, 0)
Building ...
Processing ...
Result:
  (1,0) |0> + (0,0) |1> + (0,0) |2> + (0,0) |3> + (0,0) |4> + (0,0) |5> + (0,0) |6> + (0,0) |7>
+ (0,0) |8> + (0,0) |9> + (0,0) |10> + (0,0) |11> + (0,0) |12> + (0,0) |13> + (0,0) |14> + (0,0) |15>

  P(qubit 0)=0
  P(qubit 1)=0
  P(qubit 2)=0
  P(qubit 3)=0
```

## Release 0.2.0

- Command line options

## Release 0.1.0

- Engine kernel
