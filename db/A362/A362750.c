/*
 * A362750  Number of total dominating sets in the n-double cone graph.
 * https://oeis.org/A362750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 18*a(n-3) - 24*a(n-4) - 6*a(n-5) +
 *            27*a(n-6) - 15*a(n-7) + 13*a(n-8) - 4*a(n-9), initial values:
 *            a(1)=4, a(2)=16, a(3)=79, a(4)=336, a(5)=1144, a(6)=4351,
 *            a(7)=17224, a(8)=67936, a(9)=267919
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that cache has capacity for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, return 0 */
    ensure_cache(n);
    /* Base values for 1..9. */
    static const seq_t base[10] = {
        0, 4, 16, 79, 336, 1144,
        4351, 17224, 67936, 267919
    };
    for (idx_t i = 1; cache[i] == 0 && i <= 9; ++i) {
        if (i <= 9) cache[i] = base[i];
    }
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 9) continue;  /* already set */
        if (cache[i] != 0) continue;
        /* Compute positive part and negative part separately to avoid
           intermediate underflow in unsigned arithmetic. */
        seq_t sum_pos = 7 * cache[i-1] + 18 * cache[i-3]
                      + 27 * cache[i-6] + 13 * cache[i-8];
        seq_t sum_neg = 15 * cache[i-2] + 24 * cache[i-4]
                       + 6  * cache[i-5] + 15 * cache[i-7]
                       + 4  * cache[i-9];
        cache[i] = sum_pos - sum_neg;
    }
    return cache[n];
}
