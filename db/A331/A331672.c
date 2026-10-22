/*
 * A331672  Sum of all base-n numbers with digit sum n and length at most n.
 * https://oeis.org/A331672
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (binomial(2*n-1,n)-n) * (n^n-1)/(n-1) for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* For n < 2 the function is undefined in OEIS; return 0. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Compute C(2*n-1, n) - n. */
    seq_t binom = binomial(2*n - 1, n);
    seq_t part1 = binom - (seq_t)n;

    /* Compute (n^n - 1) / (n - 1). */
    seq_t pow_n_n = pow_uint((uint64_t)n, n);
    seq_t part2 = (pow_n_n - (seq_t)1) / ((seq_t)n - (seq_t)1);

    /* Final product. */
    return part1 * part2;
}
