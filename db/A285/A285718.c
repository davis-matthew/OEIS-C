/*
 * A285718  a(1) = 0, and for n > 1, a(n) = the least squarefree number x such
 *          that n-x is also squarefree.
 * https://oeis.org/A285718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least squarefree x such that n‑x is squarefree
 * Range:     u8 n <= 3303, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t n_val = (seq_t)n;
    for (idx_t x = 1; x < n; ++x) {
        seq_t x_val = (seq_t)x;
        if (is_squarefree(x_val) && is_squarefree(n_val - x_val))
            return x_val;
    }
    /* For any n > 1 the loop should always find a value. */
    return (seq_t)0;
}
