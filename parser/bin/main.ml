open Gc_parser.Vm
open Gc_parser.Parse
open Gc_parser.Ast

let usage_msg = "fun (--repl | <file>) [--trace]"

let heap_size   = ref 2048
let threshold   = ref 80
let max_roots   = ref 10
let max_rounds  = ref 1000000000000
let stack_size  = ref 1000000000000

let input_file = ref ""

let anon_fun filename =
  input_file := filename

let spec_list =
  [("", Arg.Tuple [
    Arg.Set_int    heap_size;
    Arg.Set_int    threshold;
    Arg.Set_int    max_roots;
    Arg.Set_int    max_rounds;
    Arg.Set_int    stack_size;
    Arg.Set_string input_file;
  ], "Interactive environment.")]

let () =
  Arg.parse spec_list anon_fun usage_msg;

  let program = parse_from_file !input_file in

  vm !heap_size !threshold !max_roots !max_rounds !stack_size (char_pointer_of_int_list ~l: (op ~l: program)) (List.length program)
