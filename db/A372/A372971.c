/*
 * A372971  a(1)=1, then a(n) = floor(n/min(a(n-1),a(floor(n/2)))).
 * https://oeis.org/A372971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1)=1; a(n)=floor(n / min(a(n-1), a(floor(n/2)))) for n≥2
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Cached memoisation structure                                       */
/* ------------------------------------------------------------------ */
static seq_t *__seq_cache = NULL;          /* array of a(i) values   */
static size_t __seq_cache_cap = 0;         /* allocated capacity      */
static size_t __seq_cache_len = 0;         /* largest index+1 that's been stored */

/* Ensure the cache can hold at least `need` elements (indices 0..need-1). */
static void __seq_cache_grow(idx_t need)
{
    /* `need` is the number of elements required; compare with current capacity. */
    if ((size_t)need <= __seq_cache_cap)
        return;                          /* already sufficient */

    size_t new_cap = __seq_cache_cap ? __seq_cache_cap * 2 : 4;
    while (new_cap < (size_t)need)
        new_cap *= 2;

    seq_t *new_buf = NULL;
#if defined(__cplusplus)
    new_buf = (seq_t*)::operator new(new_cap * sizeof(seq_t));
#else
    new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (new_buf == NULL) {
        /* Out of memory – terminate the program (the build does not expect this). */
        exit(1);
    }

    /* Copy any already‑computed values. */
    if (__seq_cache) {
        memcpy(new_buf, __seq_cache, __seq_cache_len * sizeof(seq_t));
#if defined(__cplusplus)
        ::operator delete(__seq_cache);
#else
        free(__seq_cache);
#endif
    }

    /* Initialise the unused 0‑th element to 0 for safety. */
    new_buf[0] = (seq_t)0;
    __seq_cache = new_buf;
    __seq_cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n==0, harmless */

    /* Make sure the cache can store up to index n. */
    __seq_cache_grow((idx_t)(n + 1));

    /* Compute all missing values up to n (O(n) overall). */
    if (__seq_cache_len <= (size_t)n) {
        /* Start computing from the first missing index. */
        idx_t start = (idx_t)__seq_cache_len;
        if (start == 0)
            start = 1;                 /* never compute a(0) */

        for (idx_t i = start; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else {
                idx_t half = i >> 1;              /* floor(i/2) */
                seq_t prev = __seq_cache[i - 1];
                seq_t alt  = __seq_cache[half];
                seq_t divisor = prev < alt ? prev : alt;
                /* divisor is guaranteed to be >= 1 (a(1)=1). */
                val = (seq_t)(((seq_t)i) / divisor);
            }
            __seq_cache[i] = val;
        }
        __seq_cache_len = (size_t)n + 1;
    }

    return __seq_cache[n];
}
