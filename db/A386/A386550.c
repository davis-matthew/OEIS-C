/*
 * A386550  Indices of hexagonal numbers that are six times another hexagonal
 *          number.
 * https://oeis.org/A386550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386550: Indices of hexagonal numbers that are six times another hexagonal number.
   The sequence satisfies the recurrence derived from the Pell equation
   X^2 - 6Y^2 = -5 with X ≡ Y ≡ 3 (mod 4).  Let X_0 = 7, Y_0 = 3.  Then
        X_{k+1} = 49*X_k + 120*Y_k
        Y_{k+1} = 20*X_k + 49*Y_k
   For k ≥ 0, the k-th non‑trivial index is
        a(k+2) = (X_k + 1) / 4
   with the special case a(1) = 0.  This yields the sequence
        0, 2, 176, 17222, 1687556, ...   (A386550)
*/

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 1) return 0;          /* a(1) = 0 */

    idx_t k = n - 2;                /* number of recurrence steps */
    seq_t X = 7;                    /* X_0 */
    seq_t Y = 3;                    /* Y_0 */

    for (idx_t i = 0; i < k; ++i) {
        seq_t newX = 49 * X + 120 * Y;
        seq_t newY = 20 * X + 49 * Y;
        X = newX;
        Y = newY;
    }

    return (X + 1) / 4;             /* a(n) = (X_k + 1) / 4 */
}
