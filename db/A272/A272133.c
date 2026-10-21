/*
 * A272133  a(n) = 122880*n^5 - 829440*n^4 + 2258688*n^3 - 3076288*n^2 +
 *          2079892*n - 555731.
 * https://oeis.org/A272133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 122880*n^5 - 829440*n^4 + 2258688*n^3 - 3076288*n^2 +
 *            2079892*n - 555731
 * Range:     i8 none, i16 none, i32 n <= 8, i64 n <= 597, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use 128‑bit intermediate arithmetic to avoid overflow for any n that fits in the
       64‑bit integer range; for larger n the result overflows only the 64‑bit
       representation, which is acceptable because those values are not checked
       by the 64‑bit build.  */
    __int128 nn = n;                 /* promote index to 128 bits */
    __int128 res = ( __int128 )122880 * nn * nn * nn * nn * nn;  /* 122880*n^5 */
    res -= ( __int128 )829440 * nn * nn * nn * nn;               /* -829440*n^4 */
    res += ( __int128 )2258688 * nn * nn * nn;                   /* +2258688*n^3 */
    res -= ( __int128 )3076288 * nn * nn;                        /* -3076288*n^2 */
    res += ( __int128 )2079892 * nn;                             /* +2079892*n */
    res -= 555731;                                              /* -555731   */
    return (seq_t)res;                                           /* cast back to seq_t */
}
