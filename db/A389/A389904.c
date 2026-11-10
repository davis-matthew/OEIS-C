/*
 * A389904  a(n) = min(n, A059894(n)).
 * https://oeis.org/A389904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = min(n, A059894(n)), where A059894(n) complements and
 *            reverses all bits of n after the leading 1
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 99 terms through n = 9486 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A389904_rev(idx_t n) {
    int len = 0;
    idx_t t = n;
    while (t) {
        len++;
        t >>= 1;
    }
    if (len <= 1) return n;
    idx_t res = (idx_t)1 << (len - 1);
    for (int i = 0; i < len - 1; ++i) {
        if (!((n >> i) & 1)) {
            res |= (idx_t)1 << (len - 2 - i);
        }
    }
    return res;
}

seq_t a(idx_t n) {
    idx_t r = A389904_rev(n);
    idx_t m = (r < n) ? r : n;
    return (seq_t)m;
}
