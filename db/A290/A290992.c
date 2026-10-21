/*
 * A290992  p-INVERT of (0,0,0,1,2,3,4,5,...), the nonnegative integers
 *          A000027 preceded by two zeros, where p(S) = 1 - S - S^2.
 * https://oeis.org/A290992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 43, u64 n <= 83, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A290992:  p-INVERT of (0,0,0,1,2,3,4,5,…) with p(S)=1‑S‑S²
    Recurrence (valid for all n≥8):
        a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2*a(n-5) + a(n-6) + a(n-8)
    Initial values:
        a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=3, a(6)=4, a(7)=7
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* sequence cache */
static idx_t max_computed = 0;/* largest index already filled */
static size_t cache_size = 0; /* allocated size of cache */

/* Allocate or grow the cache so that indices 0…n are addressable */
static void ensure_cache(idx_t n) {
    if ((size_t)n >= cache_size) {
        size_t new_sz = cache_size ? cache_size * 2 : 8;
        while (new_sz <= (size_t)n) new_sz *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure – abort */
        cache = tmp;
        cache_size = new_sz;
    }
}

/* Initialize the cache with the first 8 terms */
static void init_cache(void) {
    if (cache) return;                /* already initialized */
    ensure_cache(7);                  /* indices 0 … 7 */
    cache[0] = 0; cache[1] = 0; cache[2] = 0;
    cache[3] = 1; cache[4] = 2; cache[5] = 3;
    cache[6] = 4; cache[7] = 7;        /* a(7) */
    max_computed = 7;
}

/*  Public function required by the framework  */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= max_computed) return cache[n];

    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence is valid for i≥8; for i<8 we already have values */
        if (i < 8) {
            /* safety guard – should not happen after init_cache */
            continue;
        }
        seq_t val = cache[i - 1] * 4;       /* 4*a(i-1) */
        val -= cache[i - 2] * 6;            /* -6*a(i-2) */
        val += cache[i - 3] * 4;            /* +4*a(i-3) */
        val -= cache[i - 5] * 2;            /* -2*a(i-5) */
        val += cache[i - 6];                /* +a(i-6)   */
        val += cache[i - 8];                /* +a(i-8)   */
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
