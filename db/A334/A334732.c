/*
 * A334732  a(n) is the total number of parts in all partitions of n into
 *          consecutive parts that differ by 5.
 * https://oeis.org/A334732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all lengths m of partitions of n into consecutive
 *            parts differing by 5. For each m >= 1 with minimal sum Smin =
 *            m*(5m-3)/2 <= n, a partition exists iff (n - Smin) is divisible
 *            by m. The total number of parts is the sum of those valid m
 * Range:     u8 n <= 425, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t nSeq  = (seq_t)n;
    for (idx_t m = 1; ; ++m) {
        seq_t m1 = (seq_t)m;
        seq_t Smin = m1 * ((seq_t)5 * m1 - (seq_t)3) / 2;
        if (Smin > nSeq) break;
        seq_t diff = nSeq - Smin;
        if (diff % m1 == 0) total += m1;
    }
    return total;
}
