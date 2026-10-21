/*
 * A295725  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = 0, a(2) = -1, a(3) = 1.
 * https://oeis.org/A295725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) (n ≥ 4), a(0) =
 *            0, a(1) = 0, a(2) = -1, a(3) = 1
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 47, i64 n <= 93, i128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for already-computed terms.  The array grows as needed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache can store terms up to index n. */
static void ensure_cache(idx_t n) {
    if (n < cache_cap) return;          /* already large enough */
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;  /* In a production system we would handle the error. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    ensure_cache(n);

    if (n < cache_len)          /* already computed */
        return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)              cache[i] = (seq_t)0;
        else if (i == 1)          cache[i] = (seq_t)0;
        else if (i == 2)          cache[i] = (seq_t)-1;
        else if (i == 3)          cache[i] = (seq_t)1;
        else                      /* recurrence */
            cache[i] = cache[i-1]
                     + (seq_t)3 * cache[i-2]
                     - (seq_t)2 * cache[i-3]
                     - (seq_t)2 * cache[i-4];
    }
    cache_len = n + 1;
    return cache[n];
}
