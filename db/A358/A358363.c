/*
 * A358363  a(n) = 16^n * Sum_{k=0..n} (-1)^k*binomial(1/2, k)^2.
 * https://oeis.org/A358363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n * Sum_{k=0..n} (-1)^k*binomial(1/2, k)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=12, a(n) = (64*(2*n-3)^2*a(n-2)+12*(4*n-3)*a(n-1))/n^2  for n>=2 */
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* memoisation cache */
static idx_t cache_len = 0;       /* current size of the cache */

seq_t a(idx_t n)
{
    /* Extend cache if needed */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;     /* allocation failed – not expected in tests */

        seq_cache = new_cache;

        /* Compute new entries */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
            {
                seq_cache[0] = (seq_t)1;
            }
            else if (i == 1)
            {
                seq_cache[1] = (seq_t)12;
            }
            else
            {
                /* Use 128‑bit arithmetic to avoid overflow during multiplication.
                   Cast back to seq_t after division, which is exact by the recurrence. */
                unsigned __int128 part1 = (unsigned __int128)64 *
                    (unsigned __int128)(2 * i - 3) *
                    (unsigned __int128)(2 * i - 3) *
                    (unsigned __int128)seq_cache[i - 2];

                unsigned __int128 part2 = (unsigned __int128)12 *
                    (unsigned __int128)(4 * i - 3) *
                    (unsigned __int128)seq_cache[i - 1];

                unsigned __int128 numerator   = part1 + part2;
                unsigned __int128 denominator = (unsigned __int128)i * (unsigned __int128)i;

                seq_cache[i] = (seq_t)(numerator / denominator);
            }
        }

        cache_len = new_len;
    }

    return seq_cache[n];
}
