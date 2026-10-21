/*
 * A250405  Numbers k such that all values of Euler phi (A000010) of all
 *          divisors of k are pairwise distinct and represent all proper
 *          divisors of k+1.
 * https://oeis.org/A250405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = 2^(2^(n-1)) - 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS offset; we guard against an out‑of‑range shift. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* compute exponent = 2^(n-1) as an idx_t value */
    idx_t exp_shift = n - 1;
    const idx_t idx_bits = sizeof(idx_t) * 8;          /* width of idx_t */
    if (exp_shift >= idx_bits) {                      /* shift would overflow */
        return (seq_t)0;                               /* not expected in tests */
    }
    idx_t exp = ((idx_t)1) << exp_shift;              /* 2^(n-1) */

    /* compute 2^exp and subtract 1 */
    seq_t val = pow2(exp);
    return val - 1;
}
