/*
 * A354388  Table read upward by antidiagonals: the n-th row gives the sums of
 *          each weakly decreasing nonnegative integer sequence of length n,
 *          listed in lexicographic order.
 * https://oeis.org/A354388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 9565, u32 n <= 9565, u64 n <= 9565,
 *            u128 n <= 9565, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series A354388: read upward by antidiagonals the sums of weakly
   decreasing non‑negative integer sequences of length n, listed in
   lexicographic order.  The function returns the n‑th term of this
   sequence (n ≥ 1). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* Return the sum of the c‑th weakly decreasing sequence of length r.
   The sequences are ordered lexicographically; c is 1‑indexed. */
static seq_t row_sum(idx_t r, idx_t c)
{
    seq_t remaining = (seq_t)c;   /* remaining rank in this row (1‑based) */
    seq_t sum = 0;
    seq_t max_prev = 0;           /* value of the previous component */

    for (idx_t pos = 1; pos <= r; ++pos) {
        idx_t tail = r - pos;     /* number of positions left to fill */

        if (pos == 1) {
            /* first component: no upper bound */
            for (seq_t v = 0;; ++v) {
                seq_t count = (tail == 0) ? (seq_t)1
                                          : binom((idx_t)(v + tail), (idx_t)tail);
                if (count >= remaining) {
                    sum += v;
                    max_prev = v;
                    break;
                }
                remaining -= count;
            }
        } else {
            /* subsequent components: must be ≤ max_prev */
            for (seq_t v = 0;; ++v) {
                if (v > max_prev) break;   /* safety: should never happen */
                seq_t count = (tail == 0) ? (seq_t)1
                                          : binom((idx_t)(v + tail), (idx_t)tail);
                if (count >= remaining) {
                    sum += v;
                    max_prev = v;
                    break;
                }
                remaining -= count;
            }
        }
    }

    return sum;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal index d such that n lies on it.
       The total number of elements up to diagonal d is d(d+1)/2. */
    idx_t d = 1;
    while ((seq_t)d * (d + 1) / 2 < (seq_t)n) ++d;

    idx_t prev   = d * (d - 1) / 2;   /* elements before diagonal d */
    idx_t offset = n - prev - 1;      /* 0‑based offset within diagonal */

    idx_t r = d - offset;            /* row index (sequence length) */
    idx_t c = offset + 1;            /* column index within that row */

    return row_sum(r, c);
}
