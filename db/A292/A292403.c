/*
 * A292403  p-INVERT of (1,0,0,0,0,1,0,0,0,0,0,0,...), where p(S) = 1 - S^2.
 * https://oeis.org/A292403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-7) + a(n-12) (n >= 13) Initial values:
 *            a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=0, a(5)=1, a(6)=2, a(7)=1,
 *            a(8)=4, a(9)=1, a(10)=6, a(11)=2, a(12)=8
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 93, u64 n <= 181,
 *            u128 n <= 358, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t, malloc, realloc */

static seq_t *cache = 0;          /* array holding computed values          */
static size_t cache_len = 0;      /* number of slots allocated              */
static size_t computed  = 0;      /* first index not yet computed           */

/* Ensure the cache can hold values up to 'need' (zero‑based). */
static void ensure_capacity(size_t need)
{
    if (need < cache_len) return;           /* already big enough */

    size_t new_len = need + 1;              /* need index 'need' + 1 slots           */

    seq_t *new_cache = 0;

#ifdef __cplusplus          /* C++ uses new/delete */
    new_cache = new seq_t[new_len];
    /* copy old data */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    delete[] cache;
#else                       /* C uses realloc (seq_t is an integer type) */
    new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
#endif

    cache = new_cache;
    cache_len = new_len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    size_t i = (size_t)n;                /* n is unsigned; cast to size_t */

    /* Allocate storage if necessary. */
    ensure_capacity(i);

    /* If already computed, just return. */
    if (i < computed) return cache[i];

    /* Initial values for indices 0..12. */
    static const seq_t init[13] = {
        (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)0,
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)4, (seq_t)1,
        (seq_t)6, (seq_t)2, (seq_t)8
    };

    /* Compute missing terms up to i. */
    for (size_t idx = computed; idx <= i; ++idx) {
        if (idx < 13) {
            cache[idx] = init[idx];
        } else {
            cache[idx] = cache[idx-2] + (cache[idx-7] * (seq_t)2) + cache[idx-12];
        }
    }
    computed = i + 1;       /* next index to compute */
    return cache[i];
}
