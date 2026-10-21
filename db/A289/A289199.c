/*
 * A289199  a(n) is the number of odd integers divisible by 13 in the open
 *          interval (12*(n-1)^2, 12*n^2).
 * https://oeis.org/A289199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = count of odd integers divisible by 13 in (12*(n-1)^2,
 *            12*n^2); Equivalent: a(n) = floor((7*(n^2-2*n+1))/13)+1 -
 *            floor((7*(n-1)^2)/13)-1 (computed efficiently using a simple
 *            arithmetic formula)
 * Range:     u8 n <= 4, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_odd13_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the observation that the count can be obtained
   as the difference of two floor sums over odd multiples of 13. */

#include <stdint.h>
#include "../utils/count_odd13_up_to.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* defined as 0 at offset 0 */
    seq_t nseq = (seq_t)n;
    seq_t n1   = nseq - 1;                        /* n-1, safe because n>0 */

    /* Compute low = 12*(n-1)^2 and high = 12*n^2 */
    seq_t low  = (seq_t)12 * n1 * n1;
    seq_t high = (seq_t)12 * nseq * nseq;

    /* Count odd multiples of 13 strictly less than high */
    seq_t cnt_high = count_odd13_up_to(high - 1);
    /* Count odd multiples of 13 <= low (treating low exclusive) */
    seq_t cnt_low  = count_odd13_up_to(low);

    return cnt_high - cnt_low;
}
