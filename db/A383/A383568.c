/*
 * A383568  Expansion of 1/sqrt((1-x^5)^2 - 4*x^2).
 * https://oeis.org/A383568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=0..⌊n/5⌋} [ (n‑5d) even ] * binom(2*((n‑5d)/2)+d, d)
 *            * binom(2*((n‑5d)/2), (n‑5d)/2)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t max_d = n / 5;
    for (idx_t d = 0; d <= max_d; ++d)
    {
        idx_t rem = n - 5 * d;
        if (rem & 1) continue;          /* skip if (n‑5d) odd */
        idx_t s = rem / 2;              /* s = (n‑5d)/2  */
        seq_t term1 = binomial(2 * s + d, d);
        seq_t term2 = binomial(2 * s, s);
        result += term1 * term2;
    }
    return result;
}
