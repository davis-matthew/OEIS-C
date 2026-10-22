/*
 * A311549  Coordination sequence Gal.6.132.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence Gal.6.132.1
   The first 12 terms are in the table below.  
   For n >= 13 the sequence satisfies a linear recurrence:
       a(n) = a(n-12) + 56
   Thus the values are periodic modulo 12 with an additive offset of 56
   each 12 steps.  For a multiple of 12 the offset is reduced by 1
   (because a(12)=56, not 57).  The closed form is
       r = n % 12
       if (r == 0) a(n) = 1 + 56*(n/12) - 1
       else        a(n) = base[r] + 56*((n - r)/12)
   where base[0..11] = {1,4,8,12,17,23,28,33,39,44,48,52}.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for n = 0..11 */
    static const seq_t base[12] =
    {
        1, 4, 8, 12, 17, 23, 28, 33, 39, 44, 48, 52
    };

    if (n <= 11)
        return base[n];

    idx_t r = n % 12;          /* remainder modulo 12 */
    idx_t q = (n - r) / 12;    /* number of full periods before n */

    if (r == 0)
        /* n is a multiple of 12 (n >= 12) */
        return (seq_t)1 + (seq_t)56 * q - (seq_t)1;  /* gives 56,112,168,... */

    return base[r] + (seq_t)56 * q;
}
