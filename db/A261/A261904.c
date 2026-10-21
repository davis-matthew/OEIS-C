/*
 * A261904  Largest x such that n can be written as n = x^2 + y^2 + z^2 with x
 *          >= y >= z >= 0, or -1 if no such x exists.
 * https://oeis.org/A261904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7536, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest x such that n = x^2 + y^2 + z^2 with x >= y >= z >= 0,
   or -1 if no such representation exists. */
/* Implements a straightforward search with O(sqrt(n)^2) time. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for unsigned 64-bit numbers. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0;
    for (int i = 32; i >= 0; --i) {
        idx_t candidate = x | ((idx_t)1 << i);
        if ((unsigned __int128)candidate * (unsigned __int128)candidate <= n)
            x = candidate;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)                         /* n = 0^2 + 0^2 + 0^2 */
        return 0;

    idx_t max_x = isqrt_u64(n);

    for (idx_t x = max_x; ; --x) {
        seq_t rem = (seq_t)n - (seq_t)x * (seq_t)x;
        idx_t rem_u = (idx_t)rem;          /* safe: rem <= n */

        idx_t max_y = isqrt_u64(rem_u);
        if (max_y > x) max_y = x;

        for (idx_t y = max_y; ; --y) {
            seq_t rem2 = rem - (seq_t)y * (seq_t)y;
            idx_t rem2_u = (idx_t)rem2;    /* safe: rem2 <= rem */

            idx_t z = isqrt_u64(rem2_u);
            if ((seq_t)z * (seq_t)z == rem2 && z <= y)
                return (seq_t)x;           /* found largest x */

            if (y == 0)
                break;
        }

        if (x == 0)
            break;
    }

    return -1;                               /* no representation */
}
