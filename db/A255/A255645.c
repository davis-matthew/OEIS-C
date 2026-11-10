/*
 * A255645  Partial sums of A134660.
 * https://oeis.org/A255645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=0..n} 2^popcount(3*k), i.e. partial sums of
 *            A134660(k) = 2^popcount(3k)
 * Range:     u8 n <= 20, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t k = 0; k <= n; k++) {
        uint64_t t = (uint64_t)k * 3ull;
        unsigned int bits = (unsigned int)__builtin_popcountll(t);
        s += pow2((idx_t)bits);
    }
    return s;
}
