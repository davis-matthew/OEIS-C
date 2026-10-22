/*
 * A357627  Numbers k such that the k-th composition in standard order has
 *          skew-alternating sum 0.
 * https://oeis.org/A357627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the k such that the k‑th composition in graded reverse‑lexicographic
 * order has skew‑alternating sum 0.  The function iterates k = 0,1,2,…,
 * generating each composition from the binary representation of k and
 * testing whether its skew‑alternating sum equals zero; when the nth such
 * k is found, it is returned.  No closed formula is known, so a search is
 * performed. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many qualifying k's found so far          */
    idx_t k = 0;          /* current composition index (k >= 0)             */

    while (1) {
        /* Build the composition from k.  The positions of 1‑bits (starting
         * from 0) yield the cumulative sums; first differences give the
         * parts, which are then taken in reverse order. */
        idx_t temp = k;
        idx_t bit_index = 0;            /* current bit position (starting at 0) */
        uint16_t diff[64];              /* differences (parts)                 */
        idx_t t = 0;                    /* number of parts                      */
        idx_t prev = 0;                 /* previous cumulative sum              */

        while (temp) {
            if (temp & 1ULL) {          /* bit set?                           */
                diff[t++] = (uint16_t)((bit_index + 1) - prev);
                prev = bit_index + 1;
            }
            bit_index++;
            temp >>= 1;
        }

        /* Compute the skew‑alternating sum.
         * Sign pattern: +, -, -, +, +, -, -, +, …  (period 4)   */
        long long sum = 0;              /* use signed intermediate           */
        for (idx_t i = 0; i < t; ++i) {
            long long part = diff[t - 1 - i];   /* reverse order              */
            long long sgn  = ((i & 3) == 0 || (i & 3) == 3) ? 1 : -1;
            sum += sgn * part;
        }
        /* For t==0 (k==0) the sum remains 0, which is correct.    */

        if (sum == 0) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }
}
