/*
 * A259400  a(n) = Sum_{k=0..n} 2^k*p(k), where p(k) is the partition function
 *          A000041.
 * https://oeis.org/A259400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 2^k * p(k), where p(k) is the partition
 *            function A000041. This implementation recomputes the partition
 *            array from scratch whenever a larger n is requested and uses it
 *            to accumulate the sum. It works for any seq_t defined by the
 *            build (8,16,32,64 bits or arbitrary precision)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for partition numbers p(0)..p(cap-1).  cap is always n+1
       after the last recomputation. */
    static seq_t *part = NULL;
    static idx_t  cap   = 0;

    /* If we need more terms than we have, recompute the partition
       numbers up to the new bound.  The simplest safe approach is to
       rebuild the entire array from scratch.  This takes O(n^2)
       operations, which is fast enough for n up to about 10 000. */
    if (n + 1 > cap) {
        /* Reallocate the array to hold 0..n. */
        seq_t *tmp = (seq_t *)realloc(part, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0;                    /* allocation failed */
        part = tmp;

        /* Clear the array and set the base case. */
        for (idx_t i = 0; i <= n; ++i) part[i] = 0;
        part[0] = 1;

        /* Standard DP for partitions: for each part size i add its
           contribution to all larger j. */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; ++j) {
                part[j] += part[j - i];
            }
        }
        cap = n + 1;
    }

    /* Compute a(n) = Σ 2^k * p(k). */
    seq_t sum  = 0;
    seq_t pow2 = 1;          /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        sum += pow2 * part[k];
        pow2 <<= 1;          /* next power of two */
    }
    return sum;
}
