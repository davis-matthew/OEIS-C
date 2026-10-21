/*
 * A243035  Number of entries of length n in A240601.
 * https://oeis.org/A243035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*10^(A000120(n)-1) for n>1, a(1)=10
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 0) return 0;            /* offset is 1; guard for defensive use */
    if (n == 1) return (seq_t)10;     /* special case */

    /* popcount of n */
    idx_t x = n;
    int pop = 0;
    while (x) {
        pop += x & 1;
        x >>= 1;
    }

    idx_t exp = (idx_t)(pop - 1);     /* exponent for 10^exp */
    seq_t p10 = pow10(exp);           /* 10^exp as a seq_t */

    return (seq_t)9 * p10;            /* 9 × 10^(popcount(n)-1) */
}
