/*
 * A275413  a(0)=0 and a(n) is the smallest available integer not yet present
 *          in the sequence that starts with the last digit of a(n-1)
 *          augmented by 1.
 * https://oeis.org/A275413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0. For n > 0, a(n) is the smallest unused integer whose
 *            decimal representation starts with (last_digit(a(n‑1)) + 1)
 * Range:     u8 n <= 109, u16 n <= 115, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for the generated sequence.  The cache is grown only
   forward; no deletions are performed.  This works for both C and
   C++ as long as seq_t is trivially copyable or the build provides
   a compatible big‑integer class. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms already generated */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure that the cache can hold at least `min_cap` elements. */
static void ensure_cache(size_t min_cap) {
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
        if (new_cap < min_cap) new_cap = min_cap;
#if defined(__cplusplus)
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#else
        cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache_cap = new_cap;
    }
}

/* Linear search for a value that is already in the cache. */
static int used_in_cache(seq_t x) {
    for (size_t i = 0; i < cache_len; ++i)
        if (cache[i] == x)
            return 1;
    return 0;
}

/* Return the smallest unused integer whose decimal representation
   starts with the decimal digits of `pref`.  `pref` is either 0‑9 or
   10 (the two‑digit prefix). */
static seq_t next_unused_with_prefix(int pref) {
    int m = 0;
    for (;;) {
        /* 10^m */
        seq_t p10 = 1;
        for (int i = 0; i < m; ++i) p10 *= (seq_t)10;

        seq_t start = (seq_t)pref * p10;
        seq_t end   = start + p10 - (seq_t)1;

        seq_t v = start;
        for (;; ++v) {
            if (v > end) break;            /* finished this block */
            if (!used_in_cache(v))
                return v;                  /* candidate found */
        }
        ++m;                                /* next larger block */
    }
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* Lazy initialisation: a(0) = 0. */
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Generate terms up to index n if necessary. */
    for (; cache_len <= n; ++cache_len) {
        seq_t prev = cache[cache_len - 1];
        seq_t last_digit = prev % (seq_t)10;
        int pref = (int)(last_digit + (seq_t)1);   /* pref = 1 .. 10 */

        seq_t val = next_unused_with_prefix(pref);
        ensure_cache(cache_len + 1);
        cache[cache_len] = val;
    }

    return cache[n];
}
