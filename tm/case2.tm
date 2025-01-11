; the finite set of states
#Q = {0,02,03,1,multi,findwri,searcha,writea,accept, accept1, accept2, accept3}

; the finite set of input symbols
#S = {a,b,1,#}

; the complete set of tape symbols
#G = {c}

; the start state
#q0 = 02

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions

;State 02
02 *_* *a* *r* 02b
02b *a* *** *r* 02b
02b *b* *** *r* 02b
02b *_* *b* *** 0 ; call case1

;
accept2 1*c *** r*r accept2
accept2 _*_ *** l*l accept3_
accept2 1*_ *** l*l re
accept2 _*c *** l** reject_
reject_ 1** _** l** reject_
reject_ _** *** r** reject

reject _** f_* r** reject2
reject2 _** a_* r** reject3
reject3 _** l_* r** reject4
reject4 _** s_* r** reject5
reject5 _** e_* *** halt_reject

re 1*c 1*_ l*l re
re _*_ *** r*r wrr

wrr *a* *** *rr wrr
wrr *_* *a* *** wrr2

wrr2 *a_ *a# *rr wrr3
wrr3 *_* *b* *ll wrr4
wrr3 *b* *** *rr wrr3
wrr3 *a* *** *rr wrr3
wrr4 *a# *** *ll wrr4
wrr4 *a_ *** *** wrr2
wrr4 *b* *** *ll wrr4
wrr4 *__ *** *rr cl

cl *a# **_ *rr cl
cl **_ *** *ll cl2
cl2 *a_ *** *ll cl2
cl2 *__ *** *rr 0

; State 0: find a
0 *a_ *** *r* 0
0 *b_ *** *r* 1

;State 1: find b
1 *b* *** *r* 1
1 *_* *** *l* multi

;State multi
multi *b* *_* *l* findwri

;State findwri
findwri **_ *** *** searcha
findwri **c *** **r findwri

;State searcha
searcha *b_ *** *l* searcha
searcha *a_ *** *** writea

;State writea
writea *a_ *ac *lr writea
writea *__ *** *r* 0p

;State 0p
0p *a* *** *r* 0p
0p *b* *** *r* 1
0p *_* *** *ll accept

;State accept
accept *_* *** *** accept1
accept *a* *** *l* accept

accept1 **_ *** *rr accept2
accept1 **c *** **l accept1

accept3_ 1*c _*_ l*l accept3_
accept3_ _*_ *** r*r accept3

accept3 _*_ t*_ r* accept4
accept4 _*_ r*_ r* accept5
accept5 _*_ u*_ r* accept6
accept6 _*_ e*_ ** halt_accept