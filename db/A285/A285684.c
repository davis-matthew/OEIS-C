/*
 * A285684  Characteristic sequence of the Beatty sequence, A022838, of
 *          sqrt(3).
 * https://oeis.org/A285684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⎣(n+1)*(1‑1/r)⎦ – ⎣n*n*(1‑1/r)⎦, r = √3
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the characteristic sequence of the Beatty
 * sequence ⎣k√3⎦:  a(n) = 0 if n ∈ { ⎣k√3⎦ : k≥1 },
 *                a(n) = 1 otherwise. */
 
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1)                /* value for n < 1 is irrelevant */
        return (seq_t)0;

    /* Find the largest k such that ⎣k√3⎦ ≤ n.
     * Since ⎣k√3⎦ = ⎣√(3k²)⎦, we can test k by computing
     * floor_sqrt(3*k*k). */
    idx_t low = 0, high = n;
    idx_t k = 0;                  /* candidate k */
    seq_t n_seq = (seq_t)n;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t mid_sq = (seq_t)mid * (seq_t)mid;
        seq_t term = floor_sqrt((seq_t)3 * mid_sq);

        if (term <= n_seq) {
            k = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    seq_t k_term = floor_sqrt((seq_t)3 * ((seq_t)k * (seq_t)k));
    return (k_term == n_seq) ? (seq_t)0 : (seq_t)1;
}
