/*
 * A286558  Ordinal transform of A005811.
 * https://oeis.org/A286558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k, 0 <= k <= n, with A005811(k) = A005811(n)
 *            (ordinal transform of A005811)
 * Range:     u8 n <= 1036, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 120 terms through n = 8727 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A286558_b(idx_t n) {
    idx_t g = n ^ (n >> 1);
    unsigned c = 0;
    while (g) {
        c += (unsigned)(g & 1);
        g >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    unsigned t = A286558_b(n);
    seq_t count = 0;
    for (idx_t j = 0; j <= n; j++) {
        if (A286558_b(j) == t) count++;
    }
    return count;
}
