/*
 * A258828  Least number k such that A258825(k) = n.
 * https://oeis.org/A258828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, hard, bref
 * Formula:   a(n) = least k such that A258825(k) = n; found by direct search
 *            over k (only a(0..3) are known)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 3 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A258828_count(uint64_t n)
{
    uint64_t cur = n;
    unsigned count = 0;
    uint64_t k = 0;
    while (1) {
        if (cur == k) count++;
        if (cur == 1) break;
        if ((cur & 1) == 0) cur >>= 1;
        else cur = (cur * 3 + 1) >> 1;
        k++;
    }
    return count;
}

seq_t a(idx_t n)
{
    uint64_t kk;
    if (n > 3) return 0; /* a(n) > 10^6 if it exists; no known terms */
    for (kk = 1; kk <= 100000; kk++) {
        if ((idx_t)A258828_count(kk) == n) return (seq_t)kk;
    }
    return 0;
}
