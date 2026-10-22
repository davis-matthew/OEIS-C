/*
 * A392272  a(n) = Sum_{k=0..floor(3*n/8)} binomial(k,3*n-8*k).
 * https://oeis.org/A392272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 53, u32 n <= 98, u64 n <= 189,
 *            u128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (used for efficient computation): 
   a(n) = 3*a(n-3) - 3*a(n-6) + a(n-8) + a(n-9),  n ≥ 9
   Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=0,
                   a(5)=0, a(6)=1, a(7)=0, a(8)=1 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

static seq_t *cache = (seq_t*)0;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;

static void ensure_cache(idx_t n)
{
    if (cache == (seq_t*)0) {
        cache_cap = 9;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1; cache[1] = (seq_t)0; cache[2] = (seq_t)0;
        cache[3] = (seq_t)1; cache[4] = (seq_t)0; cache[5] = (seq_t)0;
        cache[6] = (seq_t)1; cache[7] = (seq_t)0; cache[8] = (seq_t)1;
        last_computed = 8;
        return;
    }

    if ((size_t)n <= last_computed) return;

    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n <= last_computed) return cache[n];

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 8) {
            /* Base values are already set. */
            continue;
        }
        seq_t term1 = 3 * cache[i - 3];
        seq_t term2 = 3 * cache[i - 6];
        seq_t term3 = cache[i - 8];
        seq_t term4 = cache[i - 9];
        /* The recurrence produces a non‑negative result for all n. */
        cache[i] = term1 - term2 + term3 + term4;
    }
    last_computed = n;
    return cache[n];
}
