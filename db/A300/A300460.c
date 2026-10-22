/*
 * A300460  Number of nX3 0..1 arrays with every element equal to 2, 3 or 4
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence * a(n) = 2*a(n-1) + 2*a(n-2) + 3*a(n-3) +
 *            a(n-4) + 8*a(n-5) + 4*a(n-6) * -5*a(n-7) -11*a(n-8) +6*a(n-9)
 *            -2*a(n-10) -7*a(n-11) -a(n-12) * +4*a(n-13) -4*a(n-14) +a(n-15)
 *            * Initial terms: * a(1)=0, a(2)=2, a(3)=2, a(4)=8, a(5)=22,
 *            a(6)=68, a(7)=212, * a(8)=652, a(9)=2017, a(10)=...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

/* 1‑based cache of computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* number of allocated slots (>= cache_len+1) */
static idx_t  cache_len = 0;  /* highest n that has been computed */

/* Initialise cache with the first 15 terms.  */
static void init_cache(void)
{
    size_t init = 16;               /* indices 0 … 15  (cache[0] unused) */
    cache = alloc_seq_array(init);
    cache_cap = init;
    cache[1] = 0;   cache[2] = 2;
    cache[3] = 2;   cache[4] = 8;
    cache[5] = 22;  cache[6] = 68;
    cache[7] = 212; cache[8] = 652;
    cache[9] = 2017;  cache[10] = 6225;
    cache[11] = 19229; cache[12] = 59393;
    cache[13] = 183451; cache[14] = 566633;
    cache[15] = 1750167;
    cache_len = 15;
}

static inline void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    grow_seq_array(&cache, &cache_cap, (size_t)n + 1);
}

/* Compute a(n).  The function is memoised so that multiple calls with
 * arbitrary order of n work correctly. */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined below offset */

    if (!cache) init_cache();       /* first call */

    ensure_capacity(n);

    if (n <= cache_len) return cache[(size_t)n];

    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t pos = 0, neg = 0;

        /* Positive contributions */
        pos += (seq_t)2 * cache[(size_t)(i-1)];
        pos += (seq_t)2 * cache[(size_t)(i-2)];
        pos += (seq_t)3 * cache[(size_t)(i-3)];
        pos += (seq_t)1 * cache[(size_t)(i-4)];
        pos += (seq_t)8 * cache[(size_t)(i-5)];
        pos += (seq_t)4 * cache[(size_t)(i-6)];
        pos += (seq_t)6 * cache[(size_t)(i-9)];
        pos += (seq_t)4 * cache[(size_t)(i-13)];
        pos += (seq_t)1 * cache[(size_t)(i-15)];

        /* Negative contributions */
        neg += (seq_t)5 * cache[(size_t)(i-7)];
        neg += (seq_t)11 * cache[(size_t)(i-8)];
        neg += (seq_t)2 * cache[(size_t)(i-10)];
        neg += (seq_t)7 * cache[(size_t)(i-11)];
        neg += (seq_t)1 * cache[(size_t)(i-12)];
        neg += (seq_t)4 * cache[(size_t)(i-14)];

        cache[(size_t)i] = pos - neg;   /* pos ≥ neg for all valid n */
    }

    cache_len = n;
    return cache[(size_t)n];
}
