/*
 * A255309  Number of times log_2 can be applied to n until the result is
 *          either 1 or not a power of 2. Here log_2 means the base-2
 *          logarithm.
 * https://oeis.org/A255309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + a(log_2(n)) if n is a power of 2 except 1, 0
 *            otherwise; a(0)=0, a(1)=0
 * Range:     u8 n <= 8888, u16 n <= 8888, u32 n <= 8888, u64 n <= 8888,
 *            u128 n <= 8888, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t cur   = (seq_t)n;
    while (cur > 1 && (cur & (cur - 1)) == 0) {
        ++count;
        unsigned long len = bitlen_seq_t(cur);
        cur = (seq_t)(len - 1);
    }
    return count;
}
