/*
 * A306296  Numbers not of the form x^2+7*y^2+7*z^2.
 * https://oeis.org/A306296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer not representable as x^2 +
 *            7*y^2 + 7*z^2 for integers x,y,z (non‑negative). The
 *            implementation searches sequentially and memoizes results
 * Range:     u8 n <= 33, u16 n <= 1356, u32 n <= 1356, u64 n <= 1356,
 *            u128 n <= 1356, big any n
 * Helpers:   append_candidate, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/append_candidate.h"

static bool is_sum_two_squares(seq_t m)
{
    if (m == 0) return true;
    seq_t s = floor_sqrt(m);
    for (seq_t y = 0; y <= s; ++y)
    {
        seq_t t = m - y * y;
        seq_t r = floor_sqrt(t);
        if (r * r == t) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static state for memoized generation */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static seq_t  next_candidate = 2;   /* 1 is representable */

    /* Expand cache until we have at least n elements */
    while (cache_len < n)
    {
        /* test whether next_candidate is representable */
        bool representable = false;
        seq_t sqrt_candidate = floor_sqrt(next_candidate);
        for (seq_t x = 0; x <= sqrt_candidate; ++x)
        {
            seq_t rem = next_candidate - x * x;
            /* rem must be divisible by 7 and the quotient a sum of two squares */
            if (rem % (seq_t)7 == 0)
            {
                seq_t m = rem / (seq_t)7;
                if (is_sum_two_squares(m))
                {
                    representable = true;
                    break;
                }
            }
        }

        if (!representable)
            append_candidate(&cache, &cache_len, &cache_cap, next_candidate);

        ++next_candidate;
    }

    /* Return the n-th element (1‑based index) */
    return cache[n - 1];
}
