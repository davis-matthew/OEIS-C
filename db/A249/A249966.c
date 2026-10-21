/*
 * A249966  Number of length 6+5 0..n arrays with no six consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{10} q_{t-1} · binom(n - t + 11, 11), where q =
 *            {15, 4819, 208006, 2329714, 9235434, 14869326, 10156734,
 *            2833778, 273635, 5339} (OEIS G.f.: x*(15 + 4819*x + 208006*x^2 +
 *            … + 5339*x^9) / (1 - x)^12)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for n == 0 (unused). */
    if (n == 0) return 0;

    /* Coefficients q_k from the numerator polynomial of the G.f. */
    static const idx_t q[10] = {
        15ULL, 4819ULL, 208006ULL, 2329714ULL,
        9235434ULL, 14869326ULL, 10156734ULL,
        2833778ULL, 273635ULL, 5339ULL
    };

    seq_t result = 0;

    /* Sum over t = 1 … 10, but only while n ≥ t. */
    for (idx_t t = 1; t <= 10 && t <= n; ++t) {
        seq_t choose = binomial(n - t + 11, 11);   /* binom(n‑t+11, 11) */
        seq_t term   = choose * (seq_t)q[t - 1];    /* multiply by integer coefficient */
        result += term;
    }

    return result;
}
