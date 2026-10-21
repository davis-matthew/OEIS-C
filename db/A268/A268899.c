/*
 * A268899  Number of n X 3 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 36*a(n-2); a(1) = 12, a(2) = 168
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cap = 0;
static size_t last_n = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out‑of‑range indices are not used */

    size_t N = (size_t)n;

    if (N > last_n)
    {
        /* enlarge the cache as necessary */
        if (cache == NULL)
            ensure_capacity(N, &cap, (void**)&cache, sizeof(seq_t));
        else if (N > cap)
            ensure_capacity(N, &cap, (void**)&cache, sizeof(seq_t));

        /* initialise first term if needed */
        if (last_n == 0)
        {
            cache[0] = (seq_t)12;        /* a(1) */
            last_n = 1;
        }

        /* build up to N using the recurrence */
        for (size_t i = last_n + 1; i <= N; ++i)
        {
            if (i == 2)                 /* a(2) is fixed */
                cache[1] = (seq_t)168;
            else
            {
                seq_t ai_minus1 = cache[i - 2]; /* a(i-1) */
                seq_t ai_minus2 = cache[i - 3]; /* a(i-2) */
                cache[i - 1] = ai_minus1 * (seq_t)12 - ai_minus2 * (seq_t)36;
            }
        }
        last_n = N;
    }

    return cache[N - 1];                  /* a(n) */
}
