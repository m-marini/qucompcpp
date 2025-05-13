**Qucompcpp** is a C++ quantum computing emulation.

The documentaion is in the [Wiki section](https://github.com/m-marini/qucompcpp/wiki).

The executable file for linux are available in [[Downloads|Downloads]]

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

## Run the executable

The `qucomp` executable load the qunatum circuit from command line or from standard input.

```
./qucomp < ../half_adder
```
or 
```
./qucomp "ccnot(3,0,1) cnot(1,0) ccnot(3,1,2) cnot(2,1) cnot(1,0)"
```

## Quantum Gates

The syntax of quantum circuit is a list of gates.
The available gates are:

- `i(<bit>)`: the identity gate for the given bit
- `h(<bit>)`: the hadamard gate for the given bit
- `x(<bit>)`: the x (not) gate for the given bit
- `y(<bit>)`: the y gate for the given bit
- `z(<bit>)`: the z gate for the given bit
- `s(<bit>)`: the s gate for the given bit
- `t(<bit>)`: the t gate for the given bit
- `swap(<bit1>, <bit2>)`: the swap gate
- `cnot(<data> <control_bit1>)`: the control not gate
- `ccnot(<data_bit>, <control_bit1>, <control_bit2>)`: the control-control not (Toffoli) gate 

E.g.
The circuit half adder
```
In 0 ---c---c-----------c--- Out 0
        |   |           |
In 1 ---c---X---c---c---X--- Out 1
        |       |   |
In 2 -----------c---X------- Out 2 Sum (In 0 + In 1)
        |       |
In 3 ---X-------X----------- Out 1 Carry
```

is described with 

```
ccnot(3,0,1)
cnot(1,0)
ccnot(3,1,2)
cnot(2,1)
cnot(1,0)
```

---

_Release 0.1.0_