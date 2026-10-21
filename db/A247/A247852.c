/*
 * A247852  The 7th Hermite Polynomial evaluated at n: H_7(n) = 128*n^7
 *          -1344*n^5 + 3360*n^3 - 1680*n.
 * https://oeis.org/A247852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 128*n^7 - 1344*n^5 + 3360*n^3 - 1680*n Offset: 0 (a(0) =
 *            0)
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 10, i64 n <= 256, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* Use a wider signed integer type for intermediate calculations
       so that the result does not overflow the type seq_t before the
       final cast.  The constants are within 32 bit, so __int128 is
       sufficient for all n that fit in seq_t. */
    __int128 un   = (__int128)n;          /* n */
    __int128 n2   = un * un;              /* n^2 */
    __int128 n3   = n2 * un;              /* n^3 */
    __int128 n5   = n3 * n2;              /* n^5 */
    __int128 n7   = n5 * n2;              /* n^7 */

    __int128 res  = 128    * n7
                   - 1344  * n5
                   + 3360  * n3
                   - 1680  * un;

    return (seq_t)res;
}
