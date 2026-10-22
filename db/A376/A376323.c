/*
 * A376323  (1/5) times obverse convolution (3)**(2^n + 1); see Comments.
 * https://oeis.org/A376323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376323:
   a(0) = 1
   a(n) = a(n-1) * (2^n + 4)   for n >= 1
   (This follows from a(n)=2*a(n-1)*A052548(n-1) with A052548(n)=2^n+2.) */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;     /* memoised values */
static idx_t cache_cap = 0;     /* allocated size */
static idx_t cache_len = 0;     /* next index to compute (size of used data) */
static seq_t pow_two = 0;       /* 2^cache_len, the next power of two */

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory; caller must handle */
        cache[0] = (seq_t)1;             /* a(0) */
        cache_len = 1;                   /* next to compute is index 1 */
        pow_two = (seq_t)2;              /* 2^1, needed for a(1) */
    }

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap << 1; /* double */
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = pow_two + (seq_t)4;         /* 2^i + 4 */
        cache[i] = cache[i - 1] * term;          /* a(i) = a(i-1)*(2^i+4) */
        pow_two = pow_two * (seq_t)2;             /* advance 2^(i+1) */
    }
    cache_len = n + 1;   /* all indexes up to n are now valid */

    return cache[n];
}
