/*
 * A364567  a(n) = A297112(A005940(1+n)), where A297112 is the Möbius
 *          transform of A156552 [the inverse of map n -> A005940(1+n)].
 * https://oeis.org/A364567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = 2^A033265(n), where A033265(n) =
 *            floor(log2(n)) - (number of 1->0 transitions in binary n)
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 104 terms through n = 9437 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A033265_count(idx_t n) {
    idx_t decreases = 0;
    idx_t i = 0;
    idx_t prev = n & 1ULL;
    for (idx_t t = n >> 1; t; t >>= 1) {
        idx_t cur = t & 1ULL;
        if (prev == 1 && cur == 0) decreases++;
        prev = cur;
        i++;
    }
    return i - decreases;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t e = A033265_count(n);
    seq_t r = 1;
    for (idx_t k = 0; k < e; k++) r *= 2;
    return r;
}
