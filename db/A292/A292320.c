/*
 * A292320  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = 1 - S - S^3.
 * https://oeis.org/A292320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-3) - 2*a(n-4) - 3*a(n-6) + a(n-7) + a(n-9)
 *            (for n ≥ 10). Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4,
 *            a(4)=6, a(5)=12, a(6)=22, a(7)=36, a(8)=67, a(9)=122
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 39, u64 n <= 77, u128 n <= 154,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of terms stored (cache_len ≥ 10 after init) */

/* initialise cache with the first 10 known terms */
static void init_cache(void)
{
    cache_len = 10;
    cache = alloc_seq(cache_len);
    cache[0] = 1;
    cache[1] = 1;
    cache[2] = 2;
    cache[3] = 4;
    cache[4] = 6;
    cache[5] = 12;
    cache[6] = 22;
    cache[7] = 36;
    cache[8] = 67;
    cache[9] = 122;
}

/* grow the cache array to at least size target+1, preserving old values */
static void ensure_cache(size_t target)
{
    if (target + 1 <= cache_len) return;
    size_t new_len = target + 1;
    seq_t *new_cache = alloc_seq(new_len);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    free_seq(cache);
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* initialise cache on first call */
    if (cache == NULL) init_cache();

    /* return stored value if already computed */
    if (idx < cache_len) return cache[idx];

    /* remember previous length to know where to start computing */
    size_t old_len = cache_len;

    /* grow cache to accommodate the requested index */
    ensure_cache(idx);

    /* compute new terms from old_len through idx, inclusive */
    for (size_t i = old_len; i <= idx; ++i) {
        /* recurrence: a(n-1) + 4*a(n-3) - 2*a(n-4) - 3*a(n-6) + a(n-7) + a(n-9) */
        seq_t val = cache[i-1]
                  + (seq_t)4 * cache[i-3]
                  - (seq_t)2 * cache[i-4]
                  - (seq_t)3 * cache[i-6]
                  + cache[i-7]
                  + cache[i-9];
        cache[i] = val;
    }

    return cache[idx];
}
