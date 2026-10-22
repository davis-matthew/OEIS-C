/*
 * A377583  Expansion of e.g.f. (1 + x * exp(x))^4.
 * https://oeis.org/A377583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{4} [ (n!/(n-k)!) * binomial(4,k) * k^(n‑k) ]
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) /* binary exponentiation */
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    static const int binom[5] = {1, 4, 6, 4, 1};
    seq_t res = 0;
    for (int k = 0; k <= 4; ++k) {
        if ((idx_t)k > n) break;          /* terms with k > n are zero */
        seq_t prod = 1;
        for (idx_t i = 1; i <= (idx_t)k; ++i)
            prod *= (n - i + 1);          /* n!/(n-k)! */
        seq_t powk = pow_seq((seq_t)k, n - k);
        seq_t term = prod * binom[k] * powk;
        res += term;
    }
    return res;
}
