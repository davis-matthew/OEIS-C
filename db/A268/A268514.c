/*
 * A268514  a(0)=0; thereafter a(2n+1)=3*a(n)+1, a(2n)=2*a(n)+a(n-1)+1.
 * https://oeis.org/A268514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum{i=1..n} 2^{number of 0's in binary expansion of i}
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_log2_uint64, popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include "../utils/popcount_u64.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned int bits = floor_log2_uint64(i) + 1;
        unsigned int ones = popcount_u64(i);
        unsigned int zeros = bits - ones;
        res += ((seq_t)1 << zeros);
    }
    return res;
}
