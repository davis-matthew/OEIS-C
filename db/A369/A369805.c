/*
 * A369805  Expansion of 1/(1 - x^2/(1-x)^7).
 * https://oeis.org/A369805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-1+5*k, n-2*k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial is computed by multiplicative cancellation to avoid
   intermediate overflow. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        g = gcd(res, den);
        if (g > 1) {
            res /= g;
            den /= g;
        }

        res *= num;
        if (den != 1)
            res /= den;          /* den should now be 1 */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        sum += binom_idx(n - 1 + 5 * k, n - 2 * k);
    }
    return sum;
}
