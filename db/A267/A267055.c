/*
 * A267055  Decimal representation of the n-th iteration of the "Rule 93"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-2) - 84*a(n-4) + 64*a(n-6) (for n>5) a(0)=1,
 *            a(1)=3, a(2)=20, a(3)=47, a(4)=336, a(5)=703
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoized terms */
static idx_t cache_len = 0;       /* number of terms stored (cache_len>0 implies cache[0..cache_len-1] valid) */

/* initialize the cache with the first 6 values */
static void init_cache(void)
{
    cache = (seq_t *)malloc(6 * sizeof(seq_t));
    if (!cache) return;        /* allocation failure – caller will return 0 */
    cache[0] = 1;
    cache[1] = 3;
    cache[2] = 20;
    cache[3] = 47;
    cache[4] = 336;
    cache[5] = 703;
    cache_len = 6;
}

seq_t a(idx_t n)
{
    /* Ensure the cache is initialized */
    if (!cache) init_cache();
    if (!cache) return 0;     /* out‑of‑memory: return 0 as a sentinel */

    /* If the requested term is already cached, return it immediately */
    if (n < cache_len) return cache[n];

    /* Expand the cache so that it can hold indices up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;       /* allocation failure */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t part1 = cache[i - 2];
        seq_t part2 = cache[i - 4];
        seq_t part3 = cache[i - 6];
        /* Compute 21*part1 + 64*part3 - 84*part2.  Use addition first
           to avoid a negative intermediate value on unsigned types. */
        seq_t val = (seq_t)21 * part1 + (seq_t)64 * part3;
        val -= (seq_t)84 * part2;
        cache[i] = val;
    }
    cache_len = new_len;

    return cache[n];
}
