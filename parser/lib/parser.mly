%{
    open Ast
%}

%token <int> INT

%token COMMA

%token LLP
%token JLP
%token J
%token BLT
%token RND
%token SEL
%token ADD
%token DEL
%token QUIT

%token EOF

%start <Ast.term list> prog
%%

prog:
  | e = expr; EOF { e }
  ;

term:
  | LLP; i = INT { Llp i }
  | JLP; i = INT { Jlp i }
  | J;   i = INT { J i }
  | BLT; i = INT { Blt i }
  | RND; i = INT { Rnd i }
  | SEL;         { Sel }
  | ADD;         { Add }
  | DEL;         { Del }
  | QUIT;       { Quit }

expr:
  | l = separated_list(COMMA, term) { l }
  ;
