/*
 * A382081  a(n) = binomial(n,3) + 6*binomial(n,4) + 15*binomial(n,5) +
 *          15*binomial(n,6).
 * https://oeis.org/A382081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3) + 6*binomial(n,4) + 15*binomial(n,5) +
 *            15*binomial(n,6)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 (a(0) = 0) */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) in seq_t arithmetic.
   Assumes k <= n. Uses multiplicative formula to keep intermediate
   values small. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* res = res * (n - k + i) / i; */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t result = binom(n, 3);

    if (n >= 4)
        result += (seq_t)6 * binom(n, 4);

    if (n >= 5)
        result += (seq_t)15 * binom(n, 5);

    if (n >= 6)
        result += (seq_t)15 * binom(n, 6);

    return result;
}
