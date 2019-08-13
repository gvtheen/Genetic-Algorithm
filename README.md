Genetic Algorithm interface was written by C++ language, which also rely on Boost C++ library.

It has several characteristics, as follows:
(1). Use three different coding methods:  binary, real and gray coding.
(2). Selector operators contatins: roulette Wheel,random and tournament. Elitist selection was also taken into account. In order to keep the 
variety of offspring，the maximum selection was set to 5.
(3). Fitness Scaling consists of three methods:  linear, sigma and power.
(4). Mutator operators involve in different strategies. For binary and gray coding, UNIFORM and BOUNDARY were used; for real coding, NOUNIFORM
and GAUSSIAN were considered.
(5). Cross operator also considered following methods: SINGLE, MULTIPLE, UNIFORM, ARITHMETIC and UNARITHMETIC.
