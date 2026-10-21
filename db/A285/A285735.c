/*
 * A285735  a(1) = 1, and for n > 1, a(n) = the least squarefree number x such
 *          that x > n-x, and n-x is also squarefree.
 * https://oeis.org/A285735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least squarefree x ≥ n/2 such that n - x is also
 *            squarefree
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;   /* a(1) = 1 by definition */

    /* start = ceil(n/2) = (n+1)/2 for integer arithmetic */
    idx_t start = (n + 1) >> 1;
    seq_t n_seq = (seq_t)n;

    for (idx_t i = start; i <= n; ++i)
    {
        seq_t x = (seq_t)i;
        seq_t y = n_seq - x;   /* y = n - x */
        if (is_squarefree(x) && is_squarefree(y))
            return x;
    }

    /* Theorem guarantees a solution; this point should never be reached. */
    return (seq_t)0;
}
