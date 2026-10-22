/*
 * A314947  Coordination sequence Gal.6.198.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A314947:  Coordination sequence Gal.6.198.2
    a(n) = 5*n + f(n)
    f(n) is a small periodic adjustment taking values -1,0,1.
    f(n) is defined by:
        let q = n / 3,  r = n % 3,  t = q % 4
        if (r == 0) {
            if (q == 0)            f =  1;
            else if (q & 1)        f = (t == 3) ?  1 : -1;
            else                   f =  0;
        }
        else if (r == 1) {
            if ((q & 1) == 0)      f = (t == 2) ?  1 :  0;
            else                   f = (t == 3) ?  1 :  0;
        }
        else { // r == 2
            if ((q & 1) == 0)      f = (t == 0) ? -1 :  0;
            else                   f = (t == 1) ? -1 :  0;
        }
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    idx_t r = n % 3;
    idx_t t = q % 4;
    int s = 0;

    switch (r) {
        case 0:
            if (q == 0) {
                s = 1;
            } else if ((q & 1) != 0) {
                s = (t == 3) ? 1 : -1;
            } else {
                s = 0;
            }
            break;

        case 1:
            if ((q & 1) == 0) {
                s = (t == 2) ? 1 : 0;
            } else {
                s = (t == 3) ? 1 : 0;
            }
            break;

        case 2:
            if ((q & 1) == 0) {
                s = (t == 0) ? -1 : 0;
            } else {
                s = (t == 1) ? -1 : 0;
            }
            break;
    }

    /* Compute a(n) = 5*n + s */
    seq_t result = (seq_t)5 * (seq_t)n + (seq_t)s;
    return result;
}
