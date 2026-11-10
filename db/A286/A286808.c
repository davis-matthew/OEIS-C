/*
 * A286808  Positions of 0 in A286807; complement of A286809.
 * https://oeis.org/A286808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer not of the form 2k+1-floor(log2(k))
 *            (A286809); computed by a linear merge scan
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9704 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A286808_b(uint64_t k)
{
    unsigned lg = 0;
    uint64_t t = k;
    while (t > 1) { t >>= 1; lg++; }
    return 2 * k + 1 - lg;
}

seq_t a(idx_t n)
{
    uint64_t k = 1;
    uint64_t bv = A286808_b(1);
    uint64_t count = 0;
    uint64_t m;
    for (m = 1;; m++) {
        while (bv < m) {
            k++;
            bv = A286808_b(k);
        }
        if (bv != m) {
            count++;
            if (count == (uint64_t)n)
                return (seq_t)m;
        }
    }
}
