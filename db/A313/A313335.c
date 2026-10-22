/*
 * A313335  Coordination sequence Gal.5.271.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313335
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

/* OEIS A313335: Coordination Sequence Gal.5.271.2
   Formula: a(n) = 5*n + f(n)
   where k = n / 6, r = n % 6
   f(0) =  1                 if k == 0
   f(0) = -2                 otherwise
   f(1) = -1                 if k <= 1
   f(1) = -(2*k-1)           otherwise
   f(2) =  3*k-1
   f(3) =  1                 if k == 0
   f(3) = -(2*k-1)           otherwise
   f(4) =  3*k
   f(5) =  1                 if k == 0
   f(5) = -(2*k-1)           otherwise
*/

#include <stdint.h>   /* for int64_t */

seq_t a(idx_t n) {
    /* compute k and r using unsigned arithmetic */
    idx_t k = n / 6;
    idx_t r = n % 6;

    /* f is signed; its magnitude is tiny for n up to 10^4, far less than
       the limits of int64_t for any conceivable n we handle. */
    int64_t f = 0;

    switch (r) {
        case 0:
            f = (k == 0) ?  1 : -2;
            break;
        case 1:
            f = (k <= 1) ? -1 : -(int64_t)(2*k - 1);
            break;
        case 2:
            f = (int64_t)(3*k - 1);
            break;
        case 3:
            f = (k == 0) ?  1 : -(int64_t)(2*k - 1);
            break;
        case 4:
            f = (int64_t)(3*k);
            break;
        case 5:
            f = (k == 0) ?  1 : -(int64_t)(2*k - 1);
            break;
        default:
            /* unreachable */
            break;
    }

    /* base value 5*n */
    seq_t result = (seq_t)n * (seq_t)5;

    /* add f, taking care of sign */
    if (f >= 0) {
        result += (seq_t)f;
    } else {
        result -= (seq_t)(-f);
    }

    return result;
}
