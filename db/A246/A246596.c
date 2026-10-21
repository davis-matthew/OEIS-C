/*
 * A246596  Run Length Transform of Catalan numbers A000108.
 * https://oeis.org/A246596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product over lengths of runs of 1’s in the binary
 *            representation of n of Catalan(length). By convention a(0)=1
 * Range:     u8 n <= 62, u16 n <= 4093, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"   /* provides catalan(idx_t) */

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) = 1 by definition          */
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t m = n;               /* work on a copy of n               */

    while (m) {
        /* skip leading zeros on the right side of the current word   */
        while ((m & ((idx_t)1)) == 0) {
            m >>= 1;
        }
        if (m == 0)            /* no more set bits                   */
            break;

        /* count a run of consecutive 1's */
        idx_t run = 0;
        while ((m & ((idx_t)1)) == 1) {
            ++run;
            m >>= 1;
        }

        /* multiply by the Catalan number of this run length         */
        prod *= catalan(run);
    }

    return prod;
}
