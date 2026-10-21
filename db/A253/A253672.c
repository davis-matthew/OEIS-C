/*
 * A253672  Another fractal t(h)ree.
 * https://oeis.org/A253672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A253672 Defined by rows R_n of length 3(n+1). R_0 = {0,1,2}. For
 *            n≥1: R_n[0] = 0; R_n[1] = 1; R_n[3(n+1)-1] = 2; R_n[2+k] =
 *            R_{n-1}[k] + 3 for k = 0 .. 3n-1. The sequence a(n) is the
 *            concatenation of all rows R_n. Initial values: a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=0, a(4)=1, …
 * Range:     u8 n <= 9495, u16 n <= 9495, u32 n <= 9495, u64 n <= 9495,
 *            u128 n <= 9495, big any n
 * Helpers:   cumulative_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/cumulative_row.h"

static inline seq_t rec_value(idx_t r, idx_t pos) {
    /* Returns a value in row r at position pos (0‑based). */
    if (pos == 0) return 0;
    if (pos == 1) return 1;
    /* Last element of the row */
    if (pos == 3ULL * (r + 1ULL) - 1ULL) return 2;
    /* Recursive step: shift back to previous row and add 3 */
    return rec_value(r - 1, pos - 2) + (seq_t)3;
}

seq_t a(idx_t n) {
    /* Find the row r such that cumulative(r-1) ≤ n < cumulative(r) */
    idx_t low = 0, high = 1;
    while (cumulative_row(high) <= n) {
        if (high > (idx_t)(~(idx_t)0) >> 1) {
            high = (idx_t)(~(idx_t)0);
            break;
        }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cumulative_row(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev_cum = (r == 0) ? 0 : cumulative_row(r - 1);
    idx_t pos = n - prev_cum;
    return rec_value(r, pos);
}
