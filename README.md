<br />
<div align="center">
  <h3 align="center">gc</h3>
  <p align="center">
      A set of Garbage Collectors built in C.
  </p>
</div>

<!-- ABOUT THE PROJECT -->
## About

This repository contains an implementation of Mark&Sweep, Mark&Compact and Copy-Collection algorithms for BST's based on the [Garbage Collector Handbook](https://gchandbook.org/) for the Implementation of Programming Languages course at FCUP.

## Features

- Mark&Sweep;
- Mark&Compact;
- Copy-Collection;
- Parser for a small instruction set.

## Getting Started

### Prerequisites

In order to run this project from source, you will need to have OCaml, `dune`, and `opam` installed. To install them you can follow the [OCaml Documentation](https://ocaml.org/install#linux_mac_bsd).

You should also have GCC installed.

### Installation

```bash
# For mark&sweep
make lib-ms

# For mark&compact
make lib-mc

# For copy-collection
make lib-cc
```

You can execute programs written in files with the following command:
```bash
dune exec gc_parser -- <heap_size> <threshold> <max_roots> <max_rounds> <stack_size> <file_name>
```

Or use the C executables:
```bash
# For the mutator
./mutator <threshold> <max_roots> <max_rounds>

# For the virtual-machine
./vm <threshold> <max_roots> <max_rounds> <stack_size>
```

## Code Examples

There are several examples you can try in the [`examples`](./parser/examples) folder such as:

```
LLP  196;
RND  20;
SEL;
RND  196;
BLT  14;
DEL;
J    16;
ADD;
JLP  2;
QUIT
```
