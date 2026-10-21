/*
 * A271972  Expansion of (1 + 3*x)/(1 - 4*x + 7*x^2).
 * https://oeis.org/A271972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2); a(0) = 1, a(1) = 7
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 22, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)7;   /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
#ifdef __cplusplus
        /* In C++ mode the type seq_t may be an arbitrary‑precision integer,
           so direct multiplication is safe. */
        curr = 4 * prev1 - 7 * prev2;
#else
        /* In C mode use a 128‑bit intermediate to avoid overflow when
           seq_t is a 64‑bit signed integer. GCC/Clang support __int128. */
        __int128 tmp = (__int128)4 * prev1 - (__int128)7 * prev2;
        curr = (seq_t)tmp;
#endif
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
