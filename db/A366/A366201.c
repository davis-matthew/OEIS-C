/*
 * A366201  Number x such that triangular(x) AND x^2 = 0, where AND is the
 *          bitwise logical-and operation.
 * https://oeis.org/A366201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non‑negative integer x such that triangular(x)
 *            AND x^2 == 0 (bitwise AND). This implementation searches x
 *            sequentially
 * Range:     u8 n <= 11, u16 n <= 34, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined offset – return 0 */
        return 0;

    idx_t count = 0;       /* index of found numbers */
    idx_t x = 0;           /* candidate integer */

    while (1) {
        seq_t sx = (seq_t)x;                        /* cast to seq_t */
        seq_t tri = sx * (sx + 1) / 2;             /* triangular(x)   */
        seq_t sq  = sx * sx;                       /* x^2             */

        if ((tri & sq) == 0) {                     /* property holds */
            count++;
            if (count == n)
                return sx;                         /* return the number */
        }
        x++;                                       /* next candidate  */
    }
}
