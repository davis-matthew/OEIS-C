/*
 * A302762  Number of minimal total dominating sets in the n-Andrásfai graph.
 * https://oeis.org/A302762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-720 + 2732*n - 1880*n^2 + 505*n^3 - 40*n^4 + 3*n^5)/120
 *            for n > 2, a(1)=1, a(2)=5
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* compute using the polynomial, using signed 128‑bit intermediate to avoid
       overflow and underflow.  The result fits in seq_t for all n up to
       about 10 000 (the tests only go that far). */
    __int128 nn = n;
    __int128 num =
        -(__int128)720
        + (__int128)2732 * nn
        - (__int128)1880 * nn * nn
        + (__int128)505  * nn * nn * nn
        - (__int128)40   * nn * nn * nn * nn
        + (__int128)3    * nn * nn * nn * nn * nn;

    __int128 result = num / 120;
    return (seq_t)result;
}
