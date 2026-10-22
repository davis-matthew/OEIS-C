/*
 * A320367  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A320367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 16*a(n-1) -50*a(n-2) +68*a(n-3) -85*a(n-4)
 *            +12*a(n-5) -36*a(n-6) -12*a(n-7) +12*a(n-8) +16*a(n-9)
 * Initial values: a(1)=8, a(2)=128, a(3)=1612, a(4)=19840,
 *                 a(5)=245060, a(6)=3027408, a(7)=37398884,
 *                 a(8)=462004160, a(9)=5707334644.
 */
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values, zero‑based (a(1) at index 0) */
static idx_t cache_len = 0;     /* number of valid entries stored in cache */

/* ensure the cache contains at least n values; compute new ones if needed */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_len = 9;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();  /* out of memory */

        /* base values a(1)..a(9) */
        cache[0]  = (seq_t)8;
        cache[1]  = (seq_t)128;
        cache[2]  = (seq_t)1612;
        cache[3]  = (seq_t)19840;
        cache[4]  = (seq_t)245060;
        cache[5]  = (seq_t)3027408;
        cache[6]  = (seq_t)37398884;
        cache[7]  = (seq_t)462004160;
        cache[8]  = (seq_t)5707334644;
    }

    if (n <= cache_len) return;  /* already computed up to requested n */

    /* resize cache: double until it can hold n elements */
    idx_t new_len = cache_len;
    while (new_len < n) new_len <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t t5 = cache[i-5];
        seq_t t6 = cache[i-6];
        seq_t t7 = cache[i-7];
        seq_t t8 = cache[i-8];
        seq_t t9 = cache[i-9];

        cache[i] = 16*t1 - 50*t2 + 68*t3 - 85*t4
                   + 12*t5 - 36*t6 - 12*t7 + 12*t8 + 16*t9;
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but avoid out‑of‑range */
    ensure_cache(n);
    return cache[n-1];
}
