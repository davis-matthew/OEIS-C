/*
 * A382413  Numbers with at least one zero in their base-7 representation.
 * https://oeis.org/A382413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8656 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th non‑negative integer that contains a zero
   digit in its base‑7 representation (offset 1). */

#include <stdbool.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return true if v (in base 7) contains a zero digit.  */
static bool has_zero_base7_idx(idx_t v)
{
    if (v == 0)
        return true;                /* 0 itself has a zero digit */
    while (v) {
        if ((v % 7) == 0)
            return true;
        v /= 7;
    }
    return false;
}

static seq_t *cache = NULL;           /* stored terms          */
static idx_t cache_len  = 0;          /* number of terms stored */
static idx_t cache_cap  = 0;          /* allocated capacity    */
static idx_t next_candidate = 0;      /* next integer to test */

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* Resize allocation if needed. */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *new_cache = seq_realloc(cache,
                                      (size_t)cache_cap,
                                      (size_t)new_cap);
        if (!new_cache)
            abort();                 /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate terms until the cache has n entries. */
    for (; cache_len < n; ++next_candidate) {
        if (has_zero_base7_idx(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
    }
}

/* OEIS entry function. */
seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for offset 1, return 0 as safety */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
