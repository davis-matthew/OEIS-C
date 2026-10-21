/*
 * A246440  Nonnegative integers k satisfying cos(k) > sec(k+1).
 * https://oeis.org/A246440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS sequence A246440.
   The sequence consists of all non‑negative integers k such that
   cos(k) > sec(k+1).  For integer k the inequality holds precisely
   when k+1 lies in the interval (π/2+2πm , 3π/2+2πm) for some
   non‑negative integer m.  The code enumerates these intervals
   using a highly accurate rational approximation of π,
   355/113, which is sufficiently precise for all values needed
   (m up to about 7000 for n up to 10000).  No floating‑point
   arithmetic or math.h calls are used.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* π ≈ 355/113, accurate enough for the required range */
    const idx_t P   = 355;          /* numerator   */
    const idx_t Q   = 113;          /* denominator */
    const idx_t DEN = 2 * Q;        /* denominator for division by 2π   */

    idx_t count = 0;   /* number of terms already produced      */
    idx_t m = 0;       /* current block index                   */

    while (1) {
        /* Bounds for the current block m */
        idx_t num_low = P * (4 * m + 1);          /* π*(4m+1)       */
        idx_t low     = num_low / DEN;           /* floor(π*(4m+1)/2) */

        idx_t start   = low + 1;                 /* first n = k+1 in this block */

        idx_t num_high = P * (4 * m + 3);          /* π*(4m+3)       */
        idx_t high     = num_high / DEN;           /* floor(π*(4m+3)/2) */

        idx_t group_size = 0;
        if (high >= start)
            group_size = high - start + 1;        /* number of n in this block */

        if (count + group_size < n) {             /* the term is later */
            count += group_size;
            ++m;
            continue;
        }

        /* The desired term lies within this block */
        idx_t offset = n - count - 1;             /* zero‑based index */
        return (seq_t)(start - 1 + offset);      /* k = (n−1) in block */
    }
}
