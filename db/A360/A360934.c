/*
 * A360934  Expansion of e.g.f. Sum_{k>=0} exp((4^k - 1)*x) * x^k/k!.
 * https://oeis.org/A360934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (4^k - 1)^(n-k) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the integer power b^e for a seq_t base b and exponent e (e >= 0). */
static inline seq_t pow_seq(seq_t b, idx_t e)
{
    seq_t r = 1;
    seq_t x = b;
    while (e)
    {
        if (e & 1)
            r = r * x;
        x = x * x;
        e >>= 1;
    }
    return r;
}

/* Sequence A360934 */
seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t binom = 1;        /* C(n,0) */
    seq_t pow4  = 1;        /* 4^k  (k starts at 0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* base_minus1 = 4^k - 1 */
        seq_t base_minus1 = pow4 - 1;

        /* term = binom * (4^k - 1)^(n-k) */
        seq_t power = pow_seq(base_minus1, n - k);
        total = total + binom * power;

        /* update binom for next k: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        if (k < n)   /* avoid division by zero when k == n */
        {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        }

        /* advance 4^k to 4^(k+1) for next iteration */
        pow4 = pow4 * 4;
    }

    return total;
}
