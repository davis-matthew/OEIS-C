/*
 * A274613  Array T(n,k) = numerator of binomial(k,n)/2^k read by
 *          antidiagonals omitting the zeros (upper triangle), a sequence
 *          related to Jacobsthal numbers.
 * https://oeis.org/A274613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = numerator of binom(k,n)/2^k read by antidiagonals
 *            (skipping zeros)
 * Range:     u8 n <= 43, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal index s such that the cumulative number
       of terms up to s-1 does not exceed n, but the cumulative count
       up to s does.  The number of terms on antidiagonal s is
       floor(s/2)+1. */
    idx_t s = 0;
    idx_t cumulative = 0;
    while (1) {
        idx_t cnt = (s / 2) + 1;          /* terms on this antidiagonal */
        if (cumulative + cnt > n) break;
        cumulative += cnt;
        ++s;
    }

    /* Position within this antidiagonal (0 = largest row index). */
    idx_t pos   = n - cumulative;
    idx_t max_n = s / 2;              /* largest row index on this diagonal */
    idx_t r     = max_n - pos;        /* actual row index to use          */

    /* Binomial coefficient binom(k, n) where k = s - r, n = r. */
    seq_t num = binomial(s - r, r);

    /* Count the exponent of 2 in num. */
    idx_t v2 = 0;
    seq_t tmp = num;
    while ((tmp & ((seq_t)1)) == 0) {
        tmp >>= 1;
        ++v2;
    }

    /* Reduce the fraction by cancelling common powers of 2. */
    idx_t shift = (v2 < s) ? v2 : s;
    return num >> shift;
}
