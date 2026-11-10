/*
 * A268231  Indices of 1's in A047999.
 * https://oeis.org/A268231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th 1 in A047999 (offset 0); row r of
 *            Pascal mod 2 has 2^popcount(r) ones at columns k that are
 *            submasks of r; a(n) = r(r+1)/2 + k, found by scanning rows
 * Range:     u8 n <= 113, u16 n <= 9239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9804 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t rem = n - 1; /* 0-based rank among the ones */
    idx_t r = 0;
    for (;; r++) {
        int pc = 0;
        idx_t t = r;
        while (t) { pc += (int)(t & 1); t >>= 1; }
        idx_t cnt;
        if (pc >= 63) cnt = ~(idx_t)0;
        else cnt = (idx_t)1 << pc;
        if (rem < cnt) break;
        rem -= cnt;
    }
    /* rem-th submask of r in increasing order */
    idx_t k = 0, j = rem, bits = r;
    while (bits && j) {
        idx_t low = bits & (~bits + 1);
        if (j & 1) k |= low;
        j >>= 1;
        bits &= bits - 1;
    }
    return (seq_t)r * ((seq_t)r + 1) / 2 + (seq_t)k;
}
