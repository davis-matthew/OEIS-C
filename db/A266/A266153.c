/*
 * A266153  Least positive integer y such that -n = x^4 - y^3 + z^2 for some
 *          positive integers x and z, or 0 if no such y exists.
 * https://oeis.org/A266153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive y such that -n = x^4 - y^3 + z^2 * with
 *            positive integers x and z, or 0 if none exists. * The function
 *            searches y from 1 upward, checking whether * y^3 - n can be
 *            expressed as a sum of a fourth power and a square. * It
 *            terminates when it finds the minimal y or when y exceeds...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Upper bound for search (conjectured a(n) < n*(n+4)/2 for n>1) */
    idx_t bound;
    /* use 128-bit intermediate to avoid overflow for large n, but
       n is small (≤10000) so standard 64‑bit suffices */
    bound = (n * (n + 4)) / 2 + 10;

    seq_t n_s = (seq_t)n;

    for (idx_t y = 1; y <= bound; ++y)
    {
        seq_t y_s = (seq_t)y;
        /* y^3 */
        seq_t y3 = y_s * y_s * y_s;
        /* need y^3 > n to get positive sum */
        if (y3 <= n_s)
            continue;

        /* S = y^3 - n  = x^4 + z^2 */
        seq_t S = y3 - n_s;

        /* Compute maximum possible x for this S */
        seq_t sqrtS   = floor_sqrt(S);           /* floor(sqrt(S))   */
        seq_t sqrt2S  = floor_sqrt(sqrtS);       /* floor(sqrt(sqrt(S))) */
        idx_t xmax    = (idx_t)sqrt2S;           /* xmax fits in 64‑bit for our bounds */

        /* Try all x from 1 to xmax */
        for (idx_t x = 1; x <= xmax; ++x)
        {
            seq_t x_s = (seq_t)x;
            seq_t x2  = x_s * x_s;
            seq_t x4  = x2 * x2;

            /* remaining part must be a perfect square (z^2) */
            if (x4 > S)
                break;                 /* just in case */

            seq_t rem  = S - x4;
            /* rem must be >= 1 because z > 0 */
            if (rem < (seq_t)1)
                continue;

            seq_t z_s  = floor_sqrt(rem);
            if (z_s >= (seq_t)1 && z_s * z_s == rem)
                return (seq_t)y;       /* minimal y found */
        }
    }

    /* No representation found within the bound */
    return (seq_t)0;
}
