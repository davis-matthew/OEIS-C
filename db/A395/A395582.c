/*
 * A395582  a(n) = floor(r*a(n-1)) + floor(r*a(n-2)), a(0) = 0, a(1) = 1 and r
 *          = (3 + sqrt(13))/2.
 * https://oeis.org/A395582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(r*a(n-1)) + floor(r*a(n-2)), a(0) = 0, a(1) = 1,
 *            where r = (3 + sqrt(13)) / 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   floor_sqrt, sqrt128_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Forward declaration of the provided big‑integer square‑root helper. */
#include "../utils/floor_sqrt.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sqrt128_uint128.h"

/* Compute floor(r * x) with r = (3 + sqrt(13)) / 2. */
static seq_t floor_r_next(seq_t x) {
    /* For small integer types up to 8 bytes, use 128‑bit intermediates. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 xx = (unsigned __int128)x;
        unsigned __int128 prod = xx * xx;
        prod *= (unsigned __int128)13;          /* 13 * x*x  */
        unsigned __int128 s = sqrt128_uint128(prod);  /* floor(x*sqrt13) */
        unsigned __int128 res = (xx * 3 + s) / 2;      /* floor(r*x)  */
        return (seq_t)res;
    }
    /* For arbitrary‑precision seq_t, work with big‑integer arithmetic. */
    seq_t xx = x;
    seq_t prod = xx * xx;
    prod = prod * (seq_t)13;                   /* 13 * x*x  */
    seq_t s = floor_sqrt(prod);                /* floor(x*sqrt13) */
    seq_t res = (xx * (seq_t)3 + s) / 2;       /* floor(r*x)  */
    return res;
}

/* --- Cache for already computed terms --- */
static seq_t *cache   = NULL;   /* array of stored a(n) */
static size_t cache_sz = 0;     /* number of terms currently stored   */
static size_t cache_cap = 0;    /* allocated capacity                 */

/* Ensure that the cache contains terms up to index n. */
static void ensure_cache(idx_t n) {
    /* Nothing to do if already available. */
    if ((size_t)n < cache_sz) return;

    /* Grow capacity if necessary. */
    if (cache_cap == 0) cache_cap = 4;
    while (cache_cap <= (size_t)n) cache_cap <<= 1;

    /* Allocate new array if needed. */
    if (cache_cap > (size_t)cache_sz && (cache == NULL || cache_cap > cache_sz)) {
        seq_t *new_arr = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!new_arr) return; /* allocation failure – caller should not hit this */
        if (cache) {
            memcpy(new_arr, cache, cache_sz * sizeof(seq_t));
            free(cache);
        }
        cache = new_arr;
    }

    /* Initialise base cases if they are missing. */
    if (cache_sz == 0) {
        cache[0] = 0;
        cache_sz = 1;
    }
    if (cache_sz == 1 && n >= 1) {
        cache[1] = 1;
        cache_sz = 2;
    }

    /* Compute remaining terms up to n. */
    for (size_t i = cache_sz; i <= (size_t)n; ++i) {
        cache[i] = floor_r_next(cache[i-1]) + floor_r_next(cache[i-2]);
    }
    cache_sz = (size_t)n + 1;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
