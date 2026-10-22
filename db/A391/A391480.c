/*
 * A391480  Triangle read by rows: T(n,k) is the number of labeled magmas with
 *          n elements and k idempotents.
 * https://oeis.org/A391480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = binomial(r,k) * r^(r*(r-1)) *
 *            (r-1)^(r-k). This implements the triangle read by rows with
 *            offset 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 20, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binomial.h" /* seq_t binomial(idx_t n, idx_t k); */

static inline idx_t row_of(idx_t n)
{
    /* Compute r = floor((sqrt(8*n+1)-1)/2) accurately. */
    long double disc = sqrt((long double)1 + 8.0L * (long double)n);
    idx_t r = (idx_t)((disc - 1.0L) / 2.0L);

    /* Adjust for any rounding error. */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    if (r == 0)          /* only a(0) maps to r==0 and k==0 */
        return (seq_t)1;

    idx_t k = n - r * (r + 1) / 2;
    seq_t bin = binomial(r, k);

    /* r^(r*(r-1)) */
    seq_t pow_r = big_pow((seq_t)r, (seq_t)(r * (r - 1)));

    /* (r-1)^(r-k) */
    seq_t pow_rm1 = big_pow((seq_t)(r - 1), (seq_t)(r - k));

    return bin * pow_r * pow_rm1;
}
