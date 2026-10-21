/*
 * A275740  Sums of the next n consecutive nonsquare integers.
 * https://oeis.org/A275740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of next n consecutive non‑square integers
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache  = NULL;       /* cache of a(n) values */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static idx_t next_n = 1;           /* next block index to compute */
static uint64_t i_val  = 2;        /* next integer to test for non‑square */
static uint64_t sq = 1;            /* current square root */
static uint64_t next_sq = 4;       /* (sq+1)^2 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;           /* a(0) = 0 */

    /* lazy initialization of cache and auxiliary state */
    if (!cache) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        next_n = 1;
        i_val  = 2;
        sq     = 1;
        next_sq = 4;
    }

    /* Ensure the cache is large enough for n+1 entries */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = ((size_t)(n + 1)) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    while (next_n <= n) {
        idx_t need = next_n;
        idx_t rem  = need;
        seq_t sum  = (seq_t)0;

        while (rem > 0) {
            /* Skip perfect squares */
            if (i_val == next_sq) {
                ++i_val;
                ++sq;
                next_sq = (sq + 1) * (sq + 1);
                continue;
            }
            sum += (seq_t)i_val;
            ++i_val;
            --rem;
        }

        cache[next_n] = sum;
        ++next_n;
    }

    return cache[n];
}
