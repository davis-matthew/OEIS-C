/*
 * A282779  Period of cubes mod n.
 * https://oeis.org/A282779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of cubes mod n
   a(n) = n if n mod 9 ≠ 0
   a(n) = n/3 if n mod 9 = 0
   (OEIS A282779) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For all n ≥ 1, the period of k³ modulo n is n except when
       n is a multiple of 9, in which case the period is n/3. */
    if (n % 9 == 0)
        return (seq_t)(n / 3);
    else
        return (seq_t)n;
}
