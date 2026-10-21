/*
 * A276065  Sum of the asymmetry degrees of all compositions of n with parts
 *          in {1,5}.
 * https://oeis.org/A276065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{v=1}^{⌊n/5⌋} 2 * ⌊(n-4v)/2⌋ * C(n-4v-2, v-1)
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 74, u64 n <= 150,
 *            u128 n <= 306, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
        g = gcd(res, den);
        res /= g;
        den /= g;
        /* after reductions den is 1 or res is divisible by den */
        res = res * num / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t v = 1; v <= n / 5; ++v) {
        idx_t m = n - 4 * v;                 /* total parts */
        if (m < 2) continue;                 /* no contributing pairs */
        idx_t pairs = m / 2;                 /* floor(m/2) */
        idx_t a_bin = m - 2;                 /* n - 4v - 2 */
        idx_t b_bin = v - 1;
        if (a_bin < b_bin) continue;
        seq_t bin = binom(a_bin, b_bin);
        seq_t term = (seq_t)pairs * 2 * bin;
        total += term;
    }
    return total;
}
