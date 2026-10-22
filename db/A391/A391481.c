/*
 * A391481  Number of labeled magmas with n elements and no idempotents.
 * https://oeis.org/A391481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n*(n-1)) * (n-1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 0 */

#include "../utils/big_pow.h"   /* declares seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* Handle the small cases where the formula involves 0^0 or (-1)^something. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;   /* 1^(0) * 0^1 = 0 */

    seq_t sn = (seq_t)n;                 /* cast index to seq_t */
    seq_t n_minus_1 = (seq_t)(n - 1);

    /* Compute n^(n*(n-1)) */
    seq_t exp1 = sn * (sn - (seq_t)1);   /* n*(n-1) */
    seq_t part1 = big_pow(sn, exp1);

    /* Compute (n-1)^n */
    seq_t part2 = big_pow(n_minus_1, sn);

    return part1 * part2;
}
