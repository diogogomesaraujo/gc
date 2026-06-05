<br />
<div align="center">
  <h3 align="center">fun</h3>
  <p align="center">
      A core functional programming language implemented in OCaml.
  </p>
</div>

<!-- ABOUT THE PROJECT -->
## About

This repository contains a compiler from an extended call-by-value λ-Calculus to SECD-machine instructions and a stack machine that runs the compiled code. To learn more about the implementation read the [report](./report.pdf).
<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Features

- Conditionals (`ifzero then else`, `=`, `<>`, `<`, `>`, `<=`, `>=`);
- Pattern matching (`match _ with | _ -> _`);
- Arithmetic expressions (`+`, `-`, `/`, `*`);
- Lambda Functions (`\x -> _`);
- Variable Declaration (`let x := _`);
- Recursive and Non-Recursive Functions (`def f : x := _`, `def rec f : x := _`);
- Fixpoint (`fix (\g x -> _)`);
- Currying (`(\x' -> _) x`).

## Getting Started

### Prerequisites

In order to run this project from source, you will need to have OCaml, `dune`, and `opam` installed. To install them you can follow the [OCaml Documentation](https://ocaml.org/install#linux_mac_bsd).

### Command-line Tool

You can execute programs written in files with the following command:
```bash
dune exec fun -- <file_path>
```

Or use the interactive environment with:
```bash
dune exec fun -- --repl
```

And finally you can use the flag `--trace` to view all the intermediate steps of execution:
```bash
dune exec fun -- (<file_path> | --repl) --trace
```

### Documentation

1. To compile the documentation run:

```bash
dune build @doc
```

2. Then open the documentation with:

```bash
open _build/default/_doc/_html/index.html 
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Code Examples

There are several examples you can try in the [`examples`](./examples) folder such as:

1. Increment
```haskell
let incr :=
  \x -> x + 1
in incr 1
```

2. Factorial

```haskell
def rec fact : x
    := ifzero x
        then 1
        else x * fact (x - 1)
in fact 5
```

3. Fibonnaci Sequence

```haskell
def rec fib : x
    := ifzero x < 2
        then 1
        else (fib (x - 1)) + (fib (x - 2))
in fib 10
```

4. Tribonnaci Sequence
```haskell
let trib := fix (\trib x -> ifzero x then 0 else
    ifzero x <= 2
        then 1
        else trib (x - 1) + trib (x - 2) + trib (x - 3))
in trib 5
```

5. Catalan Numbers
```haskell
def rec fact : x
    := ifzero x
        then 1
        else x * fact (x - 1)
in def rec catalan : x
    := fact (2 * x) / (fact (x + 1) * fact x)
in catalan 10
```
<p align="right">(<a href="#readme-top">back to top</a>)</p>
