/*
 * A253463  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal minimum unequal to its
 *          neighbors horizontally, vertically, diagonally and antidiagonally.
 * https://oeis.org/A253463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 19, u64 n <= 44, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253463: number of (n+1)×3 0..2 arrays with the stated property.
   Recurrence: a(n) = 5·a(n‑2) + 8·a(n‑4) – 12·a(n‑6)  for n > 7.
   Initial values: a(1)=540, a(2)=800, a(3)=1400, a(4)=3072, a(5)=8536,
                    a(6)=15888, a(7)=44464.
   The function memoises previously computed terms so that calls arrive in any
   order and the running time is O(n) in the largest n requested. */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[7] = {
    540, 800, 1400, 3072, 8536, 15888, 44464
};

static seq_t *cache = NULL;       /* array of computed a(n), index is n */
static size_t cache_len = 0;      /* largest index computed (0 if none) */
static size_t cache_cap = 0;      /* allocated capacity */

/* Ensure that cache holds values up to n (indexing from 0). */
static void ensure_terms(idx_t n) {
    if ((size_t)n <= cache_len) return;   /* already available */

    size_t needed = (size_t)n + 1;        /* include index 0 unused */
    if (needed > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* fill missing initial values */
    if (cache_len < 7) {
        for (size_t i = cache_len + 1; i <= 7; ++i)
            cache[i] = init_vals[i - 1];
        cache_len = 7;
    }

    /* extend sequence using the recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        cache[i] = 5 * cache[i - 2] + 8 * cache[i - 4] - 12 * cache[i - 6];
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined for n < 1; return 0 */
    ensure_terms(n);
    return cache[(size_t)n];
}
