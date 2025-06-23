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

Quantum computing 0.3.0

Usage: ./qucomp [options]

Options
  -d --dump               Specify variable dump
  -f --file <file>        Specify qu source file
  -h --help               Print usage
  -v --version            Print version
```

```
$ ./qucomp -f ../qucomp.qu
Processing ...
13:clear();
  :^ value: 0
15:let in0 = |0>;
  :----^ value: |0>
16:let in1 = |0>;
  :----^ value: |0>
18:let in = (in1 x in0);
  :----^ value: |0>
19:let ha = CNOT(1,0) * CNOT(2,1) * CCNOT(3,1,2) * CNOT(1,0) * CCNOT(3,0,1);
  :----^ value: [ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0
  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0
  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 ]
20:let out = ha * in;
  :----^ value: |0>
22:let sum = out^ . qubit1(2,4) . out;
  :----^ value: 0
23:let carry = out^ . qubit1(3,4) . out;
  :----^ value: 0
```

## Release 0.3.0

- Added qu language

## Release 0.2.1

- Comment syntax

## Release 0.2.0

- Command line options

## Release 0.1.0

- Engine kernel
