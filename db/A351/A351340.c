/*
 * A351340  a(n) = Sum_{k=0..n} n^k * k^(n-k).
 * https://oeis.org/A351340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} n^k * k^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* idx_t and seq_t are supplied by the build environment */

/* Helper: binary exponentiation for seq_t.
   Computes base^exp for exp <= UINT64_MAX. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1)
            result *= b;
        e >>= 1;
        if (e)
            b *= b;
    }
    return result;
}

/* Main sequence function.
   Computes a(n) = Σ_{k=0..n} n^k * k^(n-k). */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t power_n = 1;          /* n^k, starts with k=0 */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term_k = pow_seq((seq_t)k, n - k); /* k^(n-k) */
        sum += power_n * term_k;
        power_n *= (seq_t)n;   /* move to next k: n^(k+1) */
    }
    return sum;
}
