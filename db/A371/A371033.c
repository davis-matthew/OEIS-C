/*
 * A371033  a(n) is the integer whose binary expansion starts with 1 and such
 *          that the runs of identical bits have lengths n, n-1, n-2, ..., 3,
 *          2, 1.
 * https://oeis.org/A371033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^(n*(n+1)/2) - 1 - a(n-1), a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below the offset – not used */

    seq_t prev = (seq_t)0;   /* a(0) for the recurrence */
    seq_t cur  = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        /* triangular number T_i = i*(i+1)/2 as seq_t */
        seq_t triangular = (seq_t)i * ((seq_t)i + 1) / 2;

        /* 2^(T_i) */
        seq_t pow_val = big_pow((seq_t)2, triangular);

        /* a(i) = 2^(T_i) - 1 - a(i-1) */
        cur = pow_val - (seq_t)1 - prev;

        prev = cur;
    }
    return cur;
}
