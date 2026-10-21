/*
 * A285900  Sum of all parts of all partitions of all positive integers <= n
 *          into consecutive parts.
 * https://oeis.org/A285900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊(n+1)/2⌋} (2k-1)/2 * floor(n/(2k-1)) * floor(1
 *            + n/(2k-1))
 * Range:     u8 n <= 14, u16 n <= 197, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; d += 2) {
        idx_t q = n / d;
        seq_t qseq = (seq_t)q;
        seq_t term = (seq_t)d * qseq * (qseq + 1) / 2;
        sum += term;
    }
    return sum;
}
