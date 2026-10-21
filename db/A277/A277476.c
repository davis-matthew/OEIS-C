/*
 * A277476  E.g.f.: -sinh(x)*LambertW(-x).
 * https://oeis.org/A277476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, n-k odd} binomial(n,k) * k^(k-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the generic power routine from the utils library. */
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 0;
    seq_t binom   = 1;          /* binomial(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient to binomial(n,k) */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        /* Add only when n-k is odd (parity differs). */
        if (((n - k) & 1u) != 0) {
            seq_t pow_val = pow_int((seq_t)k, k - 1);  /* k^(k-1) */
            result += binom * pow_val;
        }
    }
    return result;
}
