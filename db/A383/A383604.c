/*
 * A383604  Expansion of 1/( (1-x)^2 * (1-x-9*x^3) )^(1/3).
 * https://oeis.org/A383604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-9)^k * binomial(-1/3,k) *
 *            binomial(n-2k,k)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3ULL;      /* floor(n/3) */
    seq_t d = 1;                /* d_k = (-9)^k * binomial(-1/3,k)  */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* binomial(n-2k, k) is always an integer for k <= floor(n/3) */
        seq_t bin = binomial(n - 2 * k, k);
        sum += d * bin;          /* add the k‑th summand */

        if (k == maxk) break;    /* finished */

        /* Recurrence: d_{k+1} = d_k * 3 * (3k + 1) / (k + 1)  (exact division) */
        idx_t factor_idx = 3 * (3 * k + 1);           /* small integer factor */
        seq_t factor = (seq_t)factor_idx;
        d = (d * factor) / (k + 1);
    }

    return sum;
}
