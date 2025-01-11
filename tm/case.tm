; the finite set of states
#Q = {0,1,multi,findwri,searcha,writea,accept}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {c}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: find a
0 a_ ** r* 0
0 b_ ** r* 1

;State 1: find b
1 b* ** r* 1
1 _* ** l* multi

;State multi
multi b* _* l* findwri

;State findwri
findwri *_ ** ** searcha
findwri *c ** *r findwri

;State searcha
searcha b_ ** l* searcha
searcha a_ ** ** writea

;State writea
writea a_ ac lr writea
writea __ ** r* 0
