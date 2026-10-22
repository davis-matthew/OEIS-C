/*
 * A318116  Number of compositions of n into exactly n nonnegative parts <=
 *          eight.
 * https://oeis.org/A318116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ((x^9-1)/(x-1))^n =
 *            Sum_{k=0..⌊n/9⌋} (-1)^k * binomial(n,k) * binomial(2n-9k-1,
 *            n-9k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute binomial(n,k) with no intermediate overflow on unsigned arithmetic */
static seq_t binom_no_overflow(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator = n - k + i;
        seq_t denominator = i;

        /* cancel common factors with current result */
        seq_t g = gcd(res, denominator);
        res /= g;
        denominator /= g;

        /* cancel remaining common factors with numerator */
        g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;          /* should be 1 now */

        res *= numerator;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;
    idx_t maxk = n / 9;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t binom_n_k = binom_no_overflow(n, k);

        idx_t a_val = 2 * n - 9 * k - 1;
        idx_t b_val = n - 9 * k;

        seq_t binom_a_b = binom_no_overflow(a_val, b_val);

        seq_t prod = binom_n_k * binom_a_b;

        if (k & 1)
            result -= prod;
        else
            result += prod;
    }
    return result;
}
