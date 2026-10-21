/*
 * A267474  Number of length-6 0..n arrays with no following elements larger
 *          than the first repeated value.
 * https://oeis.org/A267474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n^6 + (197/60)*n^5 + 7*n^4 + (43/4)*n^3 + 10*n^2 + (149/30)*n + 1
 * This uses the equivalent integer form:
 *   a(n) = (60*n^6 + 197*n^5 + 420*n^4 + 645*n^3 + 600*n^2 + 298*n + 60) / 60
 * The computation is performed with 128‑bit intermediate arithmetic to avoid overflow
 * for all supported seq_t sizes (including the arbitrary‑precision builds).  The final
 * division by 60 yields an exact integer for every integer n >= 1.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use 128‑bit integers for the intermediate calculations. */
    __int128 x   = n;          /* cast idx_t to 128‑bit */
    __int128 x2  = x * x;      /* n^2 */
    __int128 x3  = x2 * x;     /* n^3 */
    __int128 x4  = x3 * x;     /* n^4 */
    __int128 x5  = x4 * x;     /* n^5 */
    __int128 x6  = x5 * x;     /* n^6 */

    /* Numerator of the integer form of the formula. */
    __int128 num = 60 * x6
                 + 197 * x5
                 + 420 * x4
                 + 645 * x3
                 + 600 * x2
                 + 298 * x
                 + 60;

    /* Exact division by 60. */
    return (seq_t)(num / 60);
}
