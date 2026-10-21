/*
 * A241868  Number of compositions of n such that the smallest part has
 *          multiplicity eight.
 * https://oeis.org/A241868
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n such that the smallest part
 *            occurs exactly eight times. Combinatorial derivation: For a
 *            given minimal part value k and a number r of parts larger than
 *            k, let m = r + 8 be the total number of parts. The sum of parts
 *            is n = 8k + Σ_{i=1}^{r} y_i, where each y_i > k...
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 36, u64 n <= 72, u128 n <= 153,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    seq_t total = 0;

    /* Case r = 0: all parts equal */
    if (n % 8 == 0)
        total += 1;

    /* r ≥ 1 */
    idx_t r_max = (n - 8) / 2;        /* floor((n-8)/2) */
    for (idx_t r = 1; r <= r_max; ++r) {
        seq_t C1 = binom(r + 8, 8);   /* C(r+8,8) */
        idx_t k_max = (n - r) / (r + 8);
        if (k_max == 0) continue;    /* no k satisfies t ≥ 0 */

        for (idx_t k = 1; k <= k_max; ++k) {
            idx_t t = n - k * (r + 8) - r;            /* non‑negative by construction */
            seq_t C2 = binom(t + r - 1, r - 1);      /* C(t+r-1, r-1) */
            total += C1 * C2;
        }
    }
    return total;
}
