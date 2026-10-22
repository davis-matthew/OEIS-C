/*
 * A343051  A triangle T(n,k) read by rows which can be used to calculate the
 *          area of a regular polygon with sides having length 1, provided
 *          that the polygon has an odd number of sides.
 * https://oeis.org/A343051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0)=1; for n≥1, let m=n−1. Find the smallest k such that
 *            k(k+3)/2 > m. Let d = m − (k−1)(k+2)/2 (0 ≤ d ≤ k). Then a(n) =
 *            | binomial(2k+1, 2d+1) · (2k+1)^(2d−1) · 16^(k−d) | with the
 *            convention that for d=0 the factor (2k+1)^(−1) cancels the
 *            binomial and a(n)=16^k. All arithmetic uses seq_t
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 41, u128 n <= 114,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Map index n to the appropriate polygon and coefficient. */
    idx_t m = n - 1;                /* Offset after the first term. */
    idx_t k = 1;
    while (k * (k + 3) / 2 <= m)
        ++k;                         /* Find minimal polygon index. */

    idx_t prev_terms = (k - 1) * (k + 2) / 2;
    idx_t d = m - prev_terms;       /* 0 ≤ d ≤ k */

    if (d == 0) {
        /* Highest‑degree coefficient: 16^k. */
        return pow_seq((seq_t)16, k);
    } else {
        /* General coefficient. */
        seq_t res = binomial(2 * k + 1, 2 * d + 1);
        res *= pow_seq((seq_t)(2 * k + 1), 2 * d - 1);
        res *= pow_seq((seq_t)16, k - d);
        return res;
    }
}
