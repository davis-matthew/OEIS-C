/*
 * A348574  Length of the shortest string over the alphabet {1,...,n} such
 *          that every subset of {1,...,n} appears as a substring (in some
 *          order).
 * https://oeis.org/A348574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more, changed
 * Formula:   a(n) = n + binomial(n, ceil(n/2)) - 2 for n>=2; a(1)=1
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;
    idx_t k = (n + 1) / 2;      /* ceil(n/2) */
    seq_t b = binomial(n, k);   /* holds C(n,k) */
    return (seq_t)n + b - 2;
}
