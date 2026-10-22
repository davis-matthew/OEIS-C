/*
 * A400696  a(n) = A008966(n^2 + 2).
 * https://oeis.org/A400696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 1 if n^2+2 is squarefree, 0 otherwise
 * Range:     u8 n <= 18, u16 n <= 4117, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = A008966(n^2 + 2); here A008966(n) is the characteristic
   function of square‑free numbers: 1 if n is squarefree, 0 otherwise. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* compute m = n^2 + 2 in a type that can hold the value */
    seq_t m = (seq_t)n * (seq_t)n + (seq_t)2;

    /* test whether any square divides m */
    for (seq_t s = (seq_t)2; s * s <= m; ++s) {
        if (m % (s * s) == (seq_t)0)
            return (seq_t)0;   /* not squarefree */
    }
    return (seq_t)1;           /* squarefree */
}
