/*
 * A372874  a(n) is the total number of runs of descents over all flattened
 *          Catalan words of length n.
 * https://oeis.org/A372874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27*n - 9 + (5*n + 1)*3^n)/36
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, but be harmless */
        return 0;

    seq_t n_t   = (seq_t)n;
    seq_t pow3n = pow3((idx_t)n);

    /* 27*n - 9 */
    seq_t term = (seq_t)27 * n_t - (seq_t)9;

    /* (5*n + 1)*3^n */
    seq_t part = (seq_t)5 * n_t + (seq_t)1;
    term = term + part * pow3n;

    /* divide by 36 */
    return term / (seq_t)36;
}
