/*
 * A292895  a(n) is the least positive k such that the Hamming weight of k
 *          equals the Hamming weight of k + n.
 * https://oeis.org/A292895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 8882, u32 n <= 8882, u64 n <= 8882,
 *            u128 n <= 8882, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least positive k such that popcount(k) = popcount(k + n).
 */
#include <stdbool.h>

static inline unsigned popcount_seq(seq_t x) {
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t sum = kk + (seq_t)n;
        if (popcount_seq(kk) == popcount_seq(sum))
            return kk;
    }
    return (seq_t)n; /* should never be reached */
}
