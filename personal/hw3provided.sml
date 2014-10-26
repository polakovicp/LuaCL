(* Coursera Programming Languages, Homework 3, Provided Code *)

exception NoAnswer

datatype pattern = Wildcard
		 | Variable of string
		 | UnitP
		 | ConstP of int
		 | TupleP of pattern list
		 | ConstructorP of string * pattern

datatype valu = Const of int
	      | Unit
	      | Tuple of valu list
	      | Constructor of string * valu

fun g f1 f2 p =
    let
	val r = g f1 f2
    in
	case p of
	    Wildcard          => f1 ()
	  | Variable x        => f2 x
	  | TupleP ps         => List.foldl (fn (p,i) => (r p) + i) 0 ps
	  | ConstructorP(_,p) => r p
	  | _                 => 0
    end

fun only_capitals (xs) =
    List.filter (fn x => Char.isUpper(String.sub(x, 0))) xs

fun longest_string1 (xs) =
    List.foldl (fn (x1, x2) => if String.size(x1) > String.size(x2) then x1 else x2) "" xs

fun longest_string2 (xs) =
    List.foldl (fn (x1, x2) => if String.size(x1) >= String.size(x2) then x1 else x2) "" xs

fun longest_string_helper f xs =
    List.foldl (fn (x, acc) => if f(String.size x, String.size acc) then x else acc) "" xs

val longest_string3 = longest_string_helper (fn (x, y) => x > y)

val longest_string4 = longest_string_helper (fn (x, y) => x >= y)

val longest_capitalized = longest_string1 o only_capitals

val rev_string = String.implode o List.rev o String.explode

fun first_answer f xs =
    case xs of
        (x::xs') => (case f x of
            SOME x' => x'
        |   NONE => first_answer f xs')
    | _ => raise NoAnswer

fun all_answers f xs =
    let
        fun aux (acc, lst) =
            case lst of
                [] => SOME acc
            |   (x::lst') => case f x of
                    NONE => NONE
                |   SOME y => aux(acc @ y, lst')
    in
        aux([], xs)
    end

(**** for the challenge problem only ****)

datatype typ = Anything
	     | UnitT
	     | IntT
	     | TupleT of typ list
	     | Datatype of string

(**** you can put all your code here ****)

fun count_wildcards x =
    g (fn () => 1) (fn y => 0) x

fun count_wild_and_variable_lengths x =
    g (fn () => 1) (fn y => String.size y) x

fun count_some_var s x =
    g (fn () => 0) (fn y => if x = s then 1 else 0) x

