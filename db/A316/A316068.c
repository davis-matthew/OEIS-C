/*
 * A316068  Coordination sequence Gal.5.296.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A316068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 6, a(2) = 9, a(3) = 17, a(4) = 20, a(5) = 24,
 *            a(6) = 32, a(7) = 37, a(8) = 41, a(9) = 48, a(10) = 52, a(11) =
 *            52, a(12) = 65, a(13) = 69, a(14) = 68, a(15) = 82, a(16) = 80,
 *            a(17) = 85, a(18) = 99, a(19) = 96, a(20) = 102; for n > 20:
 *            a(n) = -2*a(n-2) + a(n-3) -3*a(n-4) + 3*a(n-5...
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ensure_cache, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mul_small.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *c = NULL;
    static size_t cap = 0;
    static bool init_done = false;
    /* Initial 21 terms (indices 0 .. 20) */
    static const seq_t init[21] = {
        1ULL, 6ULL, 9ULL, 17ULL, 20ULL, 24ULL, 32ULL, 37ULL, 41ULL, 48ULL,
        52ULL, 52ULL, 65ULL, 69ULL, 68ULL, 82ULL, 80ULL, 85ULL, 99ULL, 96ULL,
        102ULL
    };

    if (n <= 20)
        return init[n];

    /* make sure cache can hold indices up to n (inclusive) */
    ensure_cache(n + 1, &c, &cap);

    if (!init_done) {
        for (idx_t i = 0; i <= 20; ++i)
            c[i] = init[i];
        init_done = true;
    }

    for (idx_t i = 21; i <= n; ++i) {
        seq_t pos = c[i - 3];
        pos += mul_small(c[i - 5], 3);
        pos += mul_small(c[i - 7], 5);
        pos += mul_small(c[i - 9], 6);
        pos += mul_small(c[i - 11], 6);
        pos += mul_small(c[i - 13], 5);
        pos += mul_small(c[i - 15], 3);
        pos += c[i - 17];

        seq_t neg = mul_small(c[i - 2], 2);
        neg += mul_small(c[i - 4], 3);
        neg += mul_small(c[i - 6], 3);
        neg += mul_small(c[i - 8], 4);
        neg += mul_small(c[i - 10], 4);
        neg += mul_small(c[i - 12], 4);
        neg += mul_small(c[i - 14], 3);
        neg += mul_small(c[i - 16], 3);
        neg += mul_small(c[i - 18], 2);
        neg += c[i - 20];

        c[i] = pos - neg;   /* pos >= neg for all n >= 21 */
    }

    return c[n];
}
