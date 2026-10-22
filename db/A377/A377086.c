/*
 * A377086  Number of fixed points under iteration of the map sending a
 *          positive integer to the product of its leading base-n digit and
 *          the sum of the squares of its base-n digits.
 * https://oeis.org/A377086
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Number of fixed points of the map f_n(x) = (leading
 *            base‑n digit of x) * (sum of squares of its base‑n digits) a(n)
 *            is computed for n >= 2
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Helper to test perfect square for a 64‑bit unsigned integer. */
static int is_square_uint64(uint64_t v)
{
    double s = sqrt((double)v);
    uint64_t r = (uint64_t)(s + 0.5);
    return r * r == v;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* fixed point 1 always exists in every base >= 2 */
    seq_t count = (seq_t)1;

    /* ---------- 2‑digit numbers (k = 1) ---------- */
    for (uint64_t d1 = 1; d1 < n; ++d1)
    {
        __int128 d1_128 = d1;
        __int128 D = (__int128)1
                     - (__int128)4 * d1_128 * d1_128 * d1_128 * d1_128
                     + (__int128)4 * d1_128 * d1_128 * n;
        if (D < 0)
            continue;

        uint64_t D_ull = (uint64_t)D;
        if (!is_square_uint64(D_ull))
            continue;

        uint64_t sqrtD = (uint64_t)(sqrt((double)D_ull) + 0.5);
        uint64_t denom = 2 * d1;

        /* plus root */
        if ((1 + sqrtD) % denom == 0)
        {
            uint64_t d0 = (1 + sqrtD) / denom;
            if (d0 < n)
                ++count;
        }
        /* minus root (only possible if sqrtD > 0) */
        if (sqrtD > 0 && (1 >= sqrtD) && (1 >= sqrtD))
        {
            uint64_t diff = 1 - sqrtD;
            if (diff % denom == 0)
            {
                uint64_t d0 = diff / denom;
                if (d0 < n)
                    ++count;
            }
        }
    }

    /* ---------- 3‑digit numbers (k = 2) ---------- */
    uint64_t n_sq = n * n;
    for (uint64_t d2 = 1; d2 < n; ++d2)
    {
        __int128 d2_128 = d2;
        __int128 d2_cubed = d2_128 * d2_128 * d2_128;
        uint64_t denom = 2 * d2;

        for (uint64_t d1 = 0; d1 < n; ++d1)
        {
            /* c = d2^3 - d2*n^2 + d2*d1^2 - d1*n */
            __int128 c = d2_cubed
                         - d2_128 * n_sq
                         + d2_128 * d1 * d1
                         - (__int128)d1 * n;

            if (c > 0)
                continue;  /* discriminant would be negative */

            __int128 D = (__int128)1 - (__int128)4 * d2_128 * c;
            if (D < 0)
                continue;
            uint64_t D_ull = (uint64_t)D;
            if (!is_square_uint64(D_ull))
                continue;

            uint64_t sqrtD = (uint64_t)(sqrt((double)D_ull) + 0.5);

            /* plus root */
            if ((1 + sqrtD) % denom == 0)
            {
                uint64_t d0 = (1 + sqrtD) / denom;
                if (d0 < n)
                    ++count;
            }
            /* minus root (only if sqrtD > 0) */
            if (sqrtD > 0 && (1 >= sqrtD))
            {
                uint64_t diff = 1 - sqrtD;
                if (diff % denom == 0)
                {
                    uint64_t d0 = diff / denom;
                    if (d0 < n)
                        ++count;
                }
            }
        }
    }

    return count;
}
