/*
 * A284759  a(n) = (Sum_{i=1..n-1} i^(n-2)) mod n^3.
 * https://oeis.org/A284759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{i=1}^{n-1} i^(n-2)) mod n^3
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 by definition */

    seq_t mod = (seq_t)n * (seq_t)n * (seq_t)n;
    seq_t sum = 0;
    idx_t exp = n - 2;              /* n>=2, so exp is safe */

    for (idx_t i = 1; i < n; ++i) {
        seq_t term = mod_pow((seq_t)i, exp, mod);
        sum += term;
        if (sum >= mod) sum -= mod; /* sum < 2*mod, a single subtraction is enough */
    }

    return sum;
}
