/*
 * A286051  Positions of 1 in A286049; complement of A286050.
 * https://oeis.org/A286051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position (1-based) of the n-th 1 in A286049, where
 *            A286049 is A010060 with each block 100 replaced by 0 (computed
 *            by direct scan)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9814 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A286051_tm(uint64_t x)
{
    int r = 0;
    while (x) {
        r ^= 1;
        x &= x - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t i = 0;
    uint64_t pos = 0;
    uint64_t cnt = 0;
    for (;;) {
        int t = A286051_tm(i);
        int out;
        if (t == 1 && A286051_tm(i + 1) == 0 && A286051_tm(i + 2) == 0) {
            out = 0;
            i += 3;
        } else {
            out = t;
            i++;
        }
        pos++;
        if (out) {
            cnt++;
            if (cnt == (uint64_t)n)
                return (seq_t)pos;
        }
    }
}
