/*
 * A332447  a(n) = A007814(A087808(n)).
 * https://oeis.org/A332447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A087808(n)); A087808(0)=0,
 *            A087808(2n)=2*A087808(n), A087808(2n+1)=A087808(n)+1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9868 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t val = 0;
    int i;
    for (i = 63; i >= 0; i--) {
        if (((n >> i) & 1) != 0) {
            val = val + 1;
        } else {
            val = val * 2;
        }
    }
    seq_t count = 0;
    if (val == 0) return count;
    while ((val & 1) == 0) {
        count++;
        val >>= 1;
    }
    return count;
}
