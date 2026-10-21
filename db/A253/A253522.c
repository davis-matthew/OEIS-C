/*
 * A253522  Number of (n+1)X(6+1) 0..2 arrays with every 2X2 subblock diagonal
 *          minimum minus antidiagonal minimum nondecreasing horizontally and
 *          diagonal maximum minus antidiagonal maximum nondecreasing
 *          vertically.
 * https://oeis.org/A253522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence * a(1) = 192393 * a(2) =
 *            666239 * a(3) = 791232 * a(4) = 930046 * a(5) = 1439791 * a(6) =
 *            2662626 * a(7) = 5591131 * a(8) = 13347833 * a(9) = 35219532 *
 *            a(10) = 98975447 * a(11) = 287819707 * a(12) = 851247242 * a(13)
 *            = 2537610573 * a(14) = 7591819430 * a(15) = 2...
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 33, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Built‑in initial values (indices 0..14 correspond to a(1)..a(15)) */
static const seq_t init[15] = {
    (seq_t)192393ULL,
    (seq_t)666239ULL,
    (seq_t)791232ULL,
    (seq_t)930046ULL,
    (seq_t)1439791ULL,
    (seq_t)2662626ULL,
    (seq_t)5591131ULL,
    (seq_t)13347833ULL,
    (seq_t)35219532ULL,
    (seq_t)98975447ULL,
    (seq_t)287819707ULL,
    (seq_t)851247242ULL,
    (seq_t)2537610573ULL,
    (seq_t)7591819430ULL,
    (seq_t)22748438826ULL
};

/* Cached values: 0‑based array; cache_len is the number of terms stored. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* used elements in cache */
static size_t cache_cap = 0;   /* current capacity of the array */

/* Ensure capacity of the cache array. */
static void ensure_cache_cap(size_t needed) {
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Return the n‑th term (n ≥ 1).  Memoised, works for any call order. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined; safe fallback */

    size_t target = (size_t)n;          /* number of terms to have stored */
    ensure_cache_cap(target);

    /* Compute terms that are not yet cached. */
    for (size_t i = cache_len; i < target; ++i) {
        if (i < 15) {
            cache[i] = init[i];
        } else {
            /* a(i+1) = recurrence using 0‑based indices */
            seq_t pos = cache[i - 1] * 9
                      + cache[i - 3] * 65
                      + cache[i - 5] * 51
                      + cache[i - 7] * 3;
            seq_t neg = cache[i - 2] * 33
                      + cache[i - 4] * 75
                      + cache[i - 6] * 19;
            cache[i] = pos - neg;  /* guaranteed non‑negative */
        }
    }

    cache_len = target;  /* update the number of stored terms */
    return cache[n - 1]; /* array index 0 corresponds to a(1) */
}
