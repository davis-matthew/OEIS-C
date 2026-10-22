/*
 * A317849  Number of states of the Finite State Automaton Gn accepting the
 *          language of maximal (or minimal) lexicographic representatives of
 *          elements in the positive braid monoid An.
 * https://oeis.org/A317849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (binomial(n+1-i, 2)+1)*Fibonacci(2*i)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset 1; n>=1 in practice */

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t k = n + 1 - i;                /* argument for binomial */
        seq_t term = binomial(k, 2) + 1;    /* C(k,2)+1  */
        seq_t f = fib((idx_t)(2 * i));      /* Fibonacci(2*i) */
        sum += term * f;
    }
    return sum;
}
