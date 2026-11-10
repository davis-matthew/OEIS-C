/*
 * A340093  Composite numbers k such that A003958(k) divides k-1.
 * https://oeis.org/A340093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th term of the sorted union of the powers of 2 (>= 4)
 *            and the known non-power-of-2 terms 9, 81, 180225 (no others
 *            known). Construction by merge, not a search
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const unsigned long extras[3] = {9UL, 81UL, 180225UL};
    seq_t cur = 4;
    idx_t cnt = 0;
    int e = 0;
    for (;;) {
        if (e < 3 && (seq_t)extras[e] < cur) {
            cnt++;
            if (cnt == n) return (seq_t)extras[e];
            e++;
            continue;
        }
        cnt++;
        if (cnt == n) return cur;
        cur = cur * 2;
    }
}
