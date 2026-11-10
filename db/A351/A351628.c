/*
 * A351628  Partial sums of A352717.
 * https://oeis.org/A351628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A352717(k), where A352717(k) is the greatest
 *            Lucas number (1, 3, 4, 7, 11, ...) not exceeding k
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A351628_a(idx_t n) {
    seq_t limit = (seq_t)n;
    seq_t total = 0;
    seq_t cur = 1, next = 3;
    while (cur <= limit) {
        seq_t hi = next - 1;
        if (hi > limit) hi = limit;
        total += cur * (hi - cur + 1);
        if (hi >= limit) break;
        seq_t nn = cur + next;
        if (nn <= next) break; /* overflow guard */
        cur = next;
        next = nn;
    }
    return total;
}

seq_t a(idx_t n) {
    return A351628_a(n);
}
