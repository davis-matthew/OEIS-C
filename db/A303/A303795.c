/*
 * A303795  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) - a(n-5) - 2*a(n-6) - 2*a(n-7)
 *            (n > 10) Initial values: a(1)=4 a(2)=12 a(3)=17 a(4)=30 a(5)=70
 *            a(6)=121 a(7)=221 a(8)=421 a(9)=800 a(10)=1511
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 68, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;          /* 1‑indexed cache array */
static size_t cache_cap = 0;         /* allocated capacity minus one (index 0 unused) */
static idx_t  cache_len  = 0;        /* highest n that has been computed */

/* Ensure the cache array can hold indices up to `need`.  */
static void ensure_cache_capacity(idx_t need) {
    if ((size_t)need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for n ≥ 1.  */
seq_t a(idx_t n) {
    if (n == 0) return 0;         /* the sequence is defined only for n ≥ 1 */

    /* Grow cache if needed.  */
    ensure_cache_capacity(n);

    /* Initialise base values once.  */
    if (cache_len < 10) {
        cache[1] = 4;   /* a(1) */
        cache[2] = 12;  /* a(2) */
        cache[3] = 17;  /* a(3) */
        cache[4] = 30;  /* a(4) */
        cache[5] = 70;  /* a(5) */
        cache[6] = 121; /* a(6) */
        cache[7] = 221; /* a(7) */
        cache[8] = 421; /* a(8) */
        cache[9] = 800; /* a(9) */
        cache[10] = 1511;/* a(10) */
        cache_len = 10;
    }

    if (n <= cache_len) return cache[(size_t)n];

    /* Compute missing terms using the recurrence.  */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t sum = cache[i-1] + cache[i-2] + (seq_t)2 * cache[i-3];
        seq_t sub = cache[i-5] + (seq_t)2 * cache[i-6] + (seq_t)2 * cache[i-7];
        cache[i] = sum - sub;
    }
    cache_len = n;
    return cache[(size_t)n];
}
