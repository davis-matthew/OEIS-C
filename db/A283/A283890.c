/*
 * A283890  Relative of Hofstadter Q-sequence: a(n) = max(0, n+10001) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) + a(n-a(n-4))
 *          for n > 0.
 * https://oeis.org/A283890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9904, u32 n <= 9904, u64 n <= 9904,
 *            u128 n <= 9904, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A283890
   a(n) = max(0, n + 10001)        for n <= 0
   a(n) = a(n - a(n-1)) +
          a(n - a(n-2)) +
          a(n - a(n-3)) +
          a(n - a(n-4))   for n > 0
   Offset 1 (a(1) is the first term).  */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;       /* cache[0] unused, cache[1..] holds a(1..). */
static size_t cache_len = 0;      /* highest n computed so far. */
static size_t cache_cap = 0;      /* allocated capacity for cache. */

/* Base values for non‑positive indices. */
static inline seq_t base_val(int64_t pos)
{
    int64_t v = pos + 10001;
    return (v > 0) ? (seq_t)v : (seq_t)0;
}

/* Retrieve a(pos).  For pos > 0 we use the static cache; for pos <= 0
   we return base_val(pos).  The function is recursive but memoises
   results to keep the cost linear. */
static inline seq_t get_val(int64_t pos)
{
    if (pos <= 0)
        return base_val(pos);

    /* pos is positive, treat it as a 64‑bit unsigned index. */
    idx_t upos = (idx_t)pos;

    /* Return cached value if available. */
    if (upos <= cache_len)
        return cache[upos];

    /* Expand cache to accommodate upos. */
    size_t needed = upos + 1;          /* we ignore index 0; array size upos+1 */
    if (needed > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        void *tmp = realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp)                /* allocation failed */
            return (seq_t)0;
        cache = (seq_t *)tmp;    /* cast safe in C and C++ */
        cache_cap = new_cap;
    }

    /* Compute missing values up to 'upos'. */
    for (idx_t i = cache_len + 1; i <= upos; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 4; ++k) {
            int64_t prev_index   = (int64_t)i - (int64_t)k;
            seq_t   prev_val     = get_val(prev_index);
            int64_t target_index = (int64_t)i - (int64_t)prev_val;
            seq_t   term         = get_val(target_index);
            sum += term;
        }
        cache[i] = sum;
    }
    cache_len = upos;
    return cache[upos];
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    /* OEIS guarantees n >= 1, but handle 0 safely. */
    if (n == 0)
        return base_val(0);
    return get_val((int64_t)n);
}
