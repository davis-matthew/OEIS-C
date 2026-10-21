/*
 * A283889  Relative of Hofstadter Q-sequence: a(n) = max(0, n+10000) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) + a(n-a(n-4))
 *          for n > 0.
 * https://oeis.org/A283889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9873, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A283889:
   a(n) = a(n−a(n−1)) + a(n−a(n−2)) + a(n−a(n−3)) + a(n−a(n−4))  for n > 0
   a(n) = max(0, n+10000)                                            for n ≤ 0   */
#include <stdlib.h>
#include <stdint.h>

/* internal memoisation storage: cache[0] = a(0), cache[1] = a(1), … */
static seq_t *cache = NULL;          /* dynamic array of a(n) */
static idx_t  cache_len = 0;         /* largest n already computed (≥0) */
static idx_t  cache_cap = 0;         /* current capacity of the array */

/* Helper: base value for n ≤ 0.  For positive n it should not be called. */
static inline seq_t base_value(int64_t idx) {
    /* a(n) = max(0, n+10000) for n ≤ 0 */
    if (idx > -10000)   return (seq_t)(idx + 10000);
    else                return 0;
}

/* Ensure the cache has room to store indices up to 'upto' (≥0). */
static void ensure_cache(idx_t upto) {
    if (upto >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap <= (size_t)upto) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);          /* out‑of‑memory */
        /* initialise newly allocated part */
        for (idx_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        /* set a(0) for the first allocation */
        if (!cache) new_cache[0] = (seq_t)10000;
        cache      = new_cache;
        cache_cap  = (idx_t)new_cap;
    }
}

/* Public function required by the framework */
seq_t a(idx_t n) {
    /* n is guaranteed ≥1 in the problem specification, but allow n=0 for safety */
    if (n == 0) return (seq_t)10000;   /* a(0) */
    ensure_cache(n);
    if (n <= cache_len) return cache[n];

    /* compute new terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 4; ++k) {
            /* value of a(i – k) */
            seq_t prev_val;
            if (i > (idx_t)k)         /* index positive */
                prev_val = cache[i - k];
            else
                prev_val = base_value((int64_t)i - k);

            /* index to look up: i – a(i–k)  */
            int64_t idx_target = (int64_t)i - (int64_t)prev_val;
            seq_t target_val;
            if (idx_target > 0)
                target_val = cache[(idx_t)idx_target];
            else
                target_val = base_value(idx_target);

            sum += target_val;
        }
        cache[i] = sum;
    }
    cache_len = n;
    return cache[n];
}
