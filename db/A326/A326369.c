/*
 * A326369  Number of tilings of an equilateral triangle of side length n with
 *          unit triangles (of side length 1) and exactly four unit "lozenges"
 *          or "diamonds" (also of side length 1).
 * https://oeis.org/A326369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/128)*(-7296 + 6704*n + 2284*n^2 - 3732*n^3 + 265*n^4 +
 *            648*n^5 - 126*n^6 - 36*n^7 + 9*n^8)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 20, u64 n <= 311, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is zero for n = 1, 2, 3. */
    if (n <= 3)
        return 0;

    /* Use a signed 128‑bit integer to avoid overflow in the intermediate
       computation (available on GCC/Clang). */
    __int128 N = (__int128)n;

    /* Evaluate the 8th‑degree polynomial by Horner’s rule. */
    __int128 val = 9;           /* coefficient of n^8   */
    val = val * N - 36;         /* -36*n^7              */
    val = val * N - 126;        /* -126*n^6             */
    val = val * N + 648;        /* +648*n^5             */
    val = val * N + 265;        /* +265*n^4             */
    val = val * N - 3732;       /* -3732*n^3            */
    val = val * N + 2284;       /* +2284*n^2            */
    val = val * N + 6704;       /* +6704*n               */
    val = val * N - 7296;       /* -7296                 */

    /* Multiply by 3 and divide by 128 to obtain the final result. */
    val = val * 3;
    val = val / 128;

    return (seq_t)val;
}
