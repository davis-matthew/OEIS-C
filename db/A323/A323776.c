/*
 * A323776  a(n) = Sum_{k = 1...n} binomial(k + 2^(n - k) - 1, k - 1).
 * https://oeis.org/A323776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 1..n} binomial(k + 2^(n - k) - 1, k - 1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 18, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 120 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

/* Compute binomial(pow + r, r) where pow = 2^(n-k) and r = k-1.
   Formula reduces to C(pow + r, r) = Π_{i=1..r} (pow + i) / i. */
static seq_t binom_pow_r(seq_t pow, idx_t r)
{
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= r; ++i)
    {
        res = (res * (pow + (seq_t)i)) / (seq_t)i;
    }
    return res;
}

/* OEIS sequence A323776 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;            /* offset 1 – undefined for n == 0 */

    seq_t total = (seq_t)0;
    seq_t pow   = pow2(n - (idx_t)1);   /* pow = 2^(n-1) */

    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        idx_t r   = k - (idx_t)1;          /* r = k-1   */
        seq_t bin = binom_pow_r(pow, r);
        total += bin;

        if (k < n)
            pow = pow / (seq_t)2;          /* next pow = 2^(n-(k+1)) */
    }
    return total;
}
