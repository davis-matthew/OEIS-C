/*
 * A393408  Sum of the middle parts in the partitions of n into 3 parts whose
 *          smallest part is even.
 * https://oeis.org/A393408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A393408: Sum of the middle parts in the partitions of n into 3 parts
 * with smallest part even.
 *
 * Formula:
 *   a(n) = Σ_{even j=2}^{⌊n/3⌋} Σ_{k=j}^{⌊(n-j)/2⌋} k
 *
 * Computation is O(n) by direct summation.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t max_j = n / 3;          /* largest possible smallest part */
    for (idx_t j = 2; j <= max_j; j += 2) {
        idx_t m = (n - j) / 2;    /* largest possible middle part */
        if (m < j) continue;      /* no valid middle part if m < j */
        seq_t jj = (seq_t)j;
        seq_t mm = (seq_t)m;
        seq_t count = mm - jj + 1;          /* number of k's */
        seq_t sum   = count * (jj + mm) / 2;/* sum of arithmetic progression */
        total += sum;
    }
    return total;
}
