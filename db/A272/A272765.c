/*
 * A272765  Expansion of (1 + 80*x + 2592*x^2 + 29360*x^3 + 138124*x^4 +
 *          295552*x^5 + 299984*x^6 + 144016*x^7 + 31146*x^8 + 2688*x^9 +
 *          72*x^10)/(1-x)^16.
 * https://oeis.org/A272765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{10} N_k * binomial(n‑k+15,15) where N =
 *            (1,80,2592,29360,138124,295552,299984,144016,31146,2688,72) (N_k
 *            are the coefficients of the numerator polynomial). Initial
 *            values a(0)…a(15) are computed by the same formula
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 46, u128 n <= 948,
 *            big any n
 * Helpers:   binom_15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: This implementation uses only integer arithmetic and works
   for any seq_t that supports +, -, *, / with integral operands.
   It does not rely on signed intermediates, hence it is
   safe for an unsigned seq_t type and for arbitrary‑precision types. */

#include <stdlib.h>
#include "../utils/binom_15.h"

/* Pre‑computed numerator coefficients. */
static const seq_t N[11] = {
    (seq_t)1,    (seq_t)80,    (seq_t)2592,   (seq_t)29360,
    (seq_t)138124,(seq_t)295552,(seq_t)299984,(seq_t)144016,
    (seq_t)31146,(seq_t)2688,  (seq_t)72
};

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    /* For n < 0 (should not happen with unsigned idx_t) we could return 0. */
    seq_t result = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= (idx_t)10; ++k) {
        if (n < k) continue;          /* term is zero when n < k because m < 15 */
        idx_t m = n - k + (idx_t)15;   /* m >= 15 */
        seq_t b = binom_15(m);         /* binomial(m,15) */
        result += N[k] * b;
    }
    return result;
}
