/*
 * A379633  Irregular triangle read by rows in which row n lists the largest
 *          parts of the partitions of n into consecutive parts (with the
 *          partitions ordered by increasing number of parts).
 * https://oeis.org/A379633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For each integer m ≥ 1, list all representations
   of m as a sum of consecutive positive integers.  For each
   representation the largest part is recorded.  The representations
   are ordered by increasing number of parts (i.e., by the length
   of the sum).  The sequence a(n) is obtained by reading these
   largest parts row by row.  This algorithm follows directly from
   the definition of the sequence; no closed‑form formula exists
   for a(n). */

#include <stdint.h>

seq_t a(idx_t N)
{
    idx_t target = N;          /* 1‑based index of the desired term   */
    idx_t count   = 0;          /* counter of terms generated so far   */

    for (idx_t n = 1; ; ++n)   /* iterate over rows (n = 1,2,3,…)   */
    {
        /* For this row, consider all possible lengths k of the sum.
           k can be at most the largest integer with k*(k+1)/2 ≤ n. */
        for (idx_t k = 1; ; ++k)
        {
            idx_t t = k * (k + 1) / 2;        /* smallest n that allows length k */
            if (t > n) break;                 /* no longer possible for this k */

            /* Compute the starting part a of a consecutive sequence
               of length k that sums to n.  We need a integer ≥ 1. */
            idx_t num = n - k * (k - 1) / 2;   /* numerator of (n - k*(k-1)/2)/k    */
            if (num % k != 0) continue;        /* a would not be integer          */
            idx_t a_start = num / k;
            if (a_start == 0) continue;        /* start must be ≥ 1                */

            seq_t largest = (seq_t)a_start + (seq_t)(k - 1); /* a_start + (k-1) */

            ++count;                           /* one more term emitted           */
            if (count == target)               /* reached the desired index      */
                return largest;
        }
    }
}
