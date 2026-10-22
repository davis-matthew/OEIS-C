/*
 * A398137  Expansion of generating function x / (1 - 3*x - x^2)^2.
 * https://oeis.org/A398137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3) - a(n-4) a(1)=1, a(2)=6,
 *            a(3)=29, a(4)=126 (order‑4 linear recurrence)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t mul_const(seq_t x, unsigned int c)
{
    seq_t res = 0;
    while (c)
    {
        if (c & 1U) {
            res += x;
        }
        c >>= 1U;
        if (c) {
            x += x;
        }
    }
    return res;
}

static seq_t *cache = NULL;          /* 1‑based cache */
static idx_t cached_up_to = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined, but harmless */

    if (n <= cached_up_to)                    /* already computed */
        return cache[n];

    /* extend cache to index n (inclusive) */
    idx_t new_size = n + 1;                   /* include index 0, unused */
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);           /* allocation failure */

    /* fill new entries */
    for (idx_t i = cached_up_to + 1; i <= n; ++i)
    {
        if (i == 1) { cache[1] = (seq_t)1; continue; }
        if (i == 2) { cache[2] = (seq_t)6; continue; }
        if (i == 3) { cache[3] = (seq_t)29; continue; }
        if (i == 4) { cache[4] = (seq_t)126; continue; }

        /* a(i) = 6*a(i-1) - 7*a(i-2) - 6*a(i-3) - a(i-4) */
        seq_t big = mul_const(cache[i-1], 6U);
        seq_t sum = 0;
        sum += mul_const(cache[i-2], 7U);
        sum += mul_const(cache[i-3], 6U);
        sum += cache[i-4];
        big -= sum;    /* safe: 6*a(i-1) > sum for all i ≥ 5 */
        cache[i] = big;
    }

    cached_up_to = n;
    return cache[n];
}
