/*
 * A264494  Number of (6+1)X(n+1) arrays of permutations of 0..n*7+6 with each
 *          element having directed index change 2,-1 1,0 2,1 0,-1 -2,-2 or
 *          -1,0.
 * https://oeis.org/A264494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) + 154*a(n-2) + 612*a(n-3)
 *                    - 744*a(n-4) -12624*a(n-5) -45792*a(n-6)
 *                    - 7488*a(n-7) + 66816*a(n-8)+235008*a(n-9)
 *                    - 995328*a(n-10)   for n > 11
 *
 * Initial values (1‑based):
 * a(1)=52, a(2)=324, a(3)=9714, a(4)=142352, a(5)=2517024,
 * a(6)=42169152, a(7)=714303376, a(8)=12077450784,
 * a(9)=204161943712, a(10)=3452345816640, a(11)=58366975597504
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;
    idx_t new_size = cache_size ? cache_size : 16;
    while (new_size <= n) new_size <<= 1;
    seq_t *p = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!p) abort();
    for (idx_t i = cache_size; i < new_size; ++i) p[i] = (seq_t)0;
    cache = p;
    cache_size = new_size;
}

/* first 11 terms */
static const seq_t init[11] = {
    (seq_t)52ULL,
    (seq_t)324ULL,
    (seq_t)9714ULL,
    (seq_t)142352ULL,
    (seq_t)2517024ULL,
    (seq_t)42169152ULL,
    (seq_t)714303376ULL,
    (seq_t)12077450784ULL,
    (seq_t)204161943712ULL,
    (seq_t)3452345816640ULL,
    (seq_t)58366975597504ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    if (n <= 11) {
        if (!cache[n]) cache[n] = init[n-1];
        return cache[n];
    }
    /* cache the first 11 terms if not already */
    for (idx_t i = 1; i <= 11; ++i)
        if (!cache[i]) cache[i] = init[i-1];

    /* compute terms 12 … n */
    for (idx_t i = 12; i <= n; ++i) {
        if (cache[i]) continue;        /* already computed */
        seq_t v1  = cache[i-1], v2  = cache[i-2],
              v3  = cache[i-3], v4  = cache[i-4],
              v5  = cache[i-5], v6  = cache[i-6],
              v7  = cache[i-7], v8  = cache[i-8],
              v9  = cache[i-9], v10 = cache[i-10];

        seq_t pos = mul_small(v1, 6U);
        pos += mul_small(v2, 154U);
        pos += mul_small(v3, 612U);
        pos += mul_small(v8, 66816U);
        pos += mul_small(v9, 235008U);

        seq_t neg = mul_small(v4, 744U);
        neg += mul_small(v5, 12624U);
        neg += mul_small(v6, 45792U);
        neg += mul_small(v7, 7488U);
        neg += mul_small(v10, 995328U);

        cache[i] = pos - neg;
    }
    return cache[n];
}
