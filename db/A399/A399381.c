/*
 * A399381  a(n) is the minimum positive integer k such that k*n-1 is
 *          squarefree.
 * https://oeis.org/A399381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min{k>0 : k*n-1 is squarefree}
 * Range:     u8 n <= 639, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>                                 /* bool */
#include "../utils/is_squarefree.h"                  /* bool is_squarefree(seq_t x); */

seq_t a(idx_t n)
{
    /* For n=0 (outside the offset) just return 0.  */
    if (n == 0)
        return (seq_t)0;

    /* Special case n=1: k=1 gives 0 which is not squarefree. */
    idx_t start_k = (n == 1) ? 2 : 1;

    for (idx_t k = start_k; ; ++k)
    {
        seq_t val = ((seq_t)k * (seq_t)n) - (seq_t)1;
        if (is_squarefree(val))
            return (seq_t)k;
    }
}
