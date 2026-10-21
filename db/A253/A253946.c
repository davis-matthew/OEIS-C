/*
 * A253946  a(n) = 6*binomial(n+1, 6).
 * https://oeis.org/A253946
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 6*binomial(n+1,6)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 5   (a(5) is the first term) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 5 the sequence is undefined. Return 0 for safety. */
    if (n < 5)
        return 0;

    /* Compute C(n+1,6) and multiply by 6. */
    seq_t res = binomial(n + 1, 6);
    res *= (seq_t)6;
    return res;
}
