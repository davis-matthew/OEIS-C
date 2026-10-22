/*
 * A314191  Coordination sequence Gal.5.292.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + floor(n/2) - floor(n/10) + c[n mod 10] where the
 *            adjustment function c is: c[0] = 1 (only when n == 0), otherwise
 *            0 c[1] = 0 c[2] = 0 c[3] = 1 c[4] = -1 c[5] = 0 c[6] = 0 c[7] =
 *            -1 c[8] = -1 c[9] = 0 This yields the OEIS sequence A314191 for
 *            all n ≥ 0
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the main part of the formula:
       5*n + floor(n/2) - floor(n/10).  All operations are
       performed in the integer type seq_t, which is an
       unsigned integer type of selectable width.  */
    seq_t result = (seq_t)n * (seq_t)5
                 + (seq_t)(n / (idx_t)2)
                 - (seq_t)(n / (idx_t)10);

    /* Apply the small adjustment that depends only on n mod 10.
       The adjustment is constant for each residue class,
       except for the special case n == 0.  */
    switch (n % (idx_t)10) {
        case 0:  /* discard the adjustment for all n>0,
                    add 1 when n==0 */
            if (n == (idx_t)0)
                result += (seq_t)1;
            break;
        case 3:
            result += (seq_t)1;
            break;
        case 4:
        case 7:
        case 8:
            result -= (seq_t)1;
            break;
        default:
            /* residues 1,2,5,6,9 have no adjustment */
            break;
    }
    return result;
}
