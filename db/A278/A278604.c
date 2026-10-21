/*
 * A278604  Number of n X 1 0..3 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly three mistakes.
 * https://oeis.org/A278604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+15,15) - (n+1)*binom(n+11,11) +
 *            binom(n+1,2)*binom(n+7,7) - binom(n+1,3)*binom(n+3,3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 21, u64 n <= 115, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes binomial coefficient C(n,k) using 128‑bit intermediates
   to avoid overflow for 64‑bit seq_t. */
static inline seq_t _binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;

    __uint128_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * ( __uint128_t )(n - k + i);   /* multiply first   */
        r = r / ( __uint128_t )i;             /* then divide exactly */
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t term1 = _binom(n + 15, 15);

    seq_t term2 = _binom(n + 11, 11);
    term2 *= (seq_t)(n + 1);

    seq_t term3 = _binom(n + 1, 2) * _binom(n + 7, 7);

    seq_t term4 = _binom(n + 1, 3) * _binom(n + 3, 3);

    seq_t result = term1;
    result -= term2;
    result += term3;
    result -= term4;

    return result;
}
