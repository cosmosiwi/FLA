; the finite set of states
#Q = {0,1,multi,findwri,searcha,writea,accept,accept1,accept2,accept3,clr,cl,I,L1,L2,L,E,END,G,A,espace,I2,N,P,U,T,0p}

; the finite set of input symbols
#S = {a,b,}

; the complete set of tape symbols
#G = {c,a,b,i,l,_,e,g,n,p,u,t}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {accept3}

; the number of tapes
#N = 2

; the transition functions

; State 0: find a
0 a_ ** r* 0
0 b_ ** r* 1

;State 1: find b
1 b* ** r* 1
1 _* ** l* multi
1 a* ** ** clr
clr a* ** r* clr
clr b* ** r* clr
clr _* ** l* cl
cl a* _* l* cl
cl b* _* l* cl
cl _* ** r* I

I _* i* r* L1
L1 _* l* r* L2
L2 _* l* r* E
E _* e* r* G
G _* g* r* A
A _* a* r* L
L _* l* r* espace
espace _* ** r* I2
I2 _* i* r* N
N _* n* r* P
P _* p* r* U
U _* u* r* T
T _* t* ** END

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
writea __ ** r* 0p

;State 0p
0p a* ** r* 0p
0p b* ** r* 1
0p _* ** ll accept

;State accept
accept _* ** ** accept1
accept a* ** l* accept

accept1 *_ ** rr accept2
accept1 *c ** *l accept1

accept2 *c c_ rr accept2
accept2 *_ __ ** accept3
