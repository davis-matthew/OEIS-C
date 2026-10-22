/*
 * A357626  Numbers k such that the reversed k-th composition in standard
 *          order has half-alternating sum 0.
 * https://oeis.org/A357626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerates non‑negative integers k and keeps those for which the
   half‑alternating sum of the reversed k‑th composition in standard order
   (graded reverse‑lexicographic, A066099) equals zero.  The k‑th composition
   is obtained from the set of positions of 1's in the reversed binary
   expansion of k.  The reversed composition is the list of first
   differences of that set after prepending 0.  The function returns the
   n‑th such k for n ≥ 1. */

#include <stdbool.h>
#include <stdlib.h>

static inline bool good_k(idx_t k) {
    seq_t sum = 0;
    unsigned diff_cnt = 0;
    int sign = 1;           /* +1 for first two diffs, then alternate */
    idx_t prev_pos = 0;
    idx_t pos = 1;          /* 1‑based positions in reversed order */
    idx_t x = k;

    while (x != 0) {
        if (x & 1) {
            seq_t diff = (seq_t)(pos - prev_pos);
            sum += (sign > 0) ? diff : -diff;
            diff_cnt++;
            if ((diff_cnt & 1) == 0) sign = -sign; /* after each pair */
            prev_pos = pos;
        }
        x >>= 1;
        pos++;
    }
    /* k == 0 gives sum 0 */
    return sum == 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but safe fall‑back */

    idx_t k = 0;
    idx_t found = 0;

    /* iterate until we find the n‑th good k */
    while (true) {
        if (good_k(k)) {
            found++;
            if (found == n) break;
        }
        k++;
    }
    return (seq_t)k;
}
