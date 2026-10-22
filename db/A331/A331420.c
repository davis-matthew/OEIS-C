/*
 * A331420  a(n) is the number of subsets of {1..n} that contain exactly 5 odd
 *          numbers.
 * https://oeis.org/A331420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of subsets of {1..n} that contain exactly 5 odd
 *            numbers * a(n) = binomial((n+1)/2, 5) * 2^((n-1)/2) for n odd *
 *            a(n) = binomial(n/2, 5) * 2^(n/2) for n even
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 37, u64 n <= 87, u128 n <= 203,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the binomial coefficient C(n,k) for small k using seq_t operations. */
static seq_t binom_small(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* Compute 2^exp by repeated multiplication. */
static seq_t pow2_seq(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r = r * 2;
    return r;
}

/* Sequence function */
seq_t a(idx_t n)
{
    idx_t odds  = (n + 1) / 2;  /* ceil(n/2) */
    idx_t evens = n / 2;        /* floor(n/2) */

    if (odds < 5)
        return 0;

    seq_t binom = binom_small(odds, 5);
    seq_t pow2  = pow2_seq(evens);

    return binom * pow2;
}
