/*
 * A376549  The nested recursive P312 function, a(n) = 3 + a(n-a(n-2)) for
 *          n>=1, a(n) = 1 for n<1.
 * https://oeis.org/A376549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 + a(n - a(n-2)), a(0)=1, a(n)=1 for n<1
 * Range:     u8 n <= 9750, u16 n <= 8852, u32 n <= 8852, u64 n <= 8852,
 *            u128 n <= 8852, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
    // For C++ the type seq_t might have a constructor; use new/delete.
    static seq_t *cache = nullptr;
    static size_t cache_cap = 0;
#else
    // For plain C use malloc/free; seq_t is an integer type.
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
#endif

/* Ensure the cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
#endif
        if (cache == NULL) { exit(EXIT_FAILURE); }
        /* Existing entries are zero; set a(0)=1 immediately. */
        cache[0] = (seq_t)1;
    } else if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (new_cache == NULL) { exit(EXIT_FAILURE); }
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
#endif
        cache_cap = new_cap;
    }
}

/* Recursive computation with memoisation.  */
static seq_t rec_a(idx_t n)
{
    ensure_cache(n);

    if (cache[n] != (seq_t)0) return cache[n];

    if (n == (idx_t)0) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    seq_t a_nm2 = (n >= (idx_t)2) ? rec_a(n - (idx_t)2) : (seq_t)1;

    idx_t sub = (n >= a_nm2) ? (n - (idx_t)a_nm2) : (idx_t)0;
    seq_t result = (seq_t)3 + rec_a(sub);

    cache[n] = result;
    return result;
}

seq_t a(idx_t n)
{
    return rec_a(n);
}
