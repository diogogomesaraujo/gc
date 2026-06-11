type term =
  | Llp of int
  | Jlp of int
  | J of int
  | Rnd of int
  | Blt of int
  | Sel
  | Del
  | Add
  | Quit

let op ~l =
  List.fold_left (
    fun acc x ->
      match x with
      | Llp i -> acc @ [1; i]
      | Jlp i -> acc @ [2; i]
      | J   i -> acc @ [3; i]
      | Blt i -> acc @ [4; i]
      | Rnd i -> acc @ [5; i]
      | Sel   -> acc @ [6; 0]
      | Add   -> acc @ [7; 0]
      | Del   -> acc @ [8; 0]
      | Quit  -> acc @ [9; 0]
  ) [] l
