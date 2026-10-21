/*
 * A264846  Irregular triangle read by rows where row n lists the least
 *          members of the equivalence classes of the equivalence relation xRy
 *          defined by floor(n/x) = floor(n/y) with n, x and y in N*.
 * https://oeis.org/A264846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = least member of each equivalence class for floor(n/x)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   row_elem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_elem.h"

static idx_t row_len(idx_t n) {
    /* number of distinct values floor(n/x) for x>=1 */
    idx_t len = 0;
    idx_t x = 1;
    seq_t prev_q = (seq_t)-1;
    while (1) {
        seq_t q = (seq_t)n / (seq_t)x;
        if (q != prev_q) {
            ++len;
            prev_q = q;
        }
        /* last distinct value is 0, obtained at x = n+1 */
        if (q == 0 && x == n + 1) break;
        ++x;
    }
    return len;
}

seq_t a(idx_t n) {
    if (n < 1U) return (seq_t)0;
    idx_t row = 1U;
    idx_t remaining = n;
    while (1) {
        idx_t len = row_len(row);
        if (remaining <= len) break;
        remaining -= len;
        ++row;
    }
    /* remaining is the position (1‑based) inside the found row */
    return row_elem(row, remaining - 1U);
}
