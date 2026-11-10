/*
 * A363686  Irregular triangle T(n, k) read by rows which represents a
 *          predecessor number system for Collatz sequences (see Comments for
 *          precise definition).
 * https://oeis.org/A363686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1)=5; for n>1, a(n)=(y*2^k-1)/3 with k=1+(n mod 2),
 *            y=a(floor(n/2))+r*2^(A056792(floor(n/2))+2), r the least in
 *            {0,1,2} making a(n) an integer
 * Range:     u8 n <= 12, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Validated: standard, pass, 79 terms through n = 125 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A056792_a(idx_t n) {
    if (n == 0) return 0;
    unsigned pop = 0, lg = 0;
    idx_t v = n;
    while (v > 1) {
        if (v & 1) ++pop;
        v >>= 1;
        ++lg;
    }
    if (v & 1) ++pop;
    return pop + lg;
}

seq_t a(idx_t n) {
    if (n == 1) return 5;
    int top = 63;
    while (!((n >> top) & 1)) --top;
    seq_t val = 5;
    idx_t cur = 1;
    for (int i = top - 1; i >= 0; --i) {
        unsigned bit = (unsigned)((n >> i) & 1);
        unsigned k = 1 + bit;
        unsigned e = A056792_a(cur);
        seq_t M = ((seq_t)1) << (e + 2);
        seq_t y = val;
        seq_t x = 0;
        for (int r = 0; r < 3; ++r) {
            seq_t t = (y << k) - 1;
            if (t % 3 == 0) {
                x = t / 3;
                break;
            }
            y += M;
        }
        val = x;
        cur = cur * 2 + bit;
    }
    return val;
}
