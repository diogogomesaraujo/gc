{
open Parser
}

let white = [' ' '\t' '\r' '\n']+
let digit = ['0'-'9']
let int = '-'? digit+

rule read =
    parse
    | white { read lexbuf }
    | int { INT (int_of_string (Lexing.lexeme lexbuf))}
    | ";" { COMMA }
    | "PAD" { PAD }
    | "LLP" { LLP }
    | "JLP" { JLP }
    | "J" { J }
    | "BLT" { BLT }
    | "RND" { RND }
    | "SEL" { SEL }
    | "ADD" { ADD }
    | "DEL" { DEL }
    | "QUIT" { QUIT }
    | eof { EOF }
