/*
 * A302966  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +15*a(n-2) -11*a(n-3) -64*a(n-4) -10*a(n-5) *
 *            +73*a(n-6) +15*a(n-7) -7*a(n-8) +4*a(n-9) -30*a(n-10) *
 *            -9*a(n-11) +8*a(n-12) for n>13 * Initial values: a(1)=4,
 *            a(2)=32, a(3)=154, a(4)=786, a(5)=3924, * a(6)=19868,
 *            a(7)=100161, a(8)=505908, a(9)=2554201, * a(10)=12898093,
 *            a(11)=6513...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[13] = {
    (seq_t)4,          /* a(1) */
    (seq_t)32,         /* a(2) */
    (seq_t)154,        /* a(3) */
    (seq_t)786,        /* a(4) */
    (seq_t)3924,       /* a(5) */
    (seq_t)19868,      /* a(6) */
    (seq_t)100161,     /* a(7) */
    (seq_t)505908,     /* a(8) */
    (seq_t)2554201,    /* a(9) */
    (seq_t)12898093,   /* a(10) */
    (seq_t)65130456,   /* a(11) */
    (seq_t)328890172,  /* a(12) */
    (seq_t)1660800969 /* a(13) */
};

static seq_t *cache = NULL;
static idx_t cached_n = 0;      /* largest n for which cache[n-1] is valid (n>=1) */
static size_t cache_cap = 0;    /* allocated capacity */

static void ensure_cache(idx_t n)
{
    if (n <= cached_n) return;

    /* Allocate / grow cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < n) {
        size_t cap = cache_cap;
        while (cap < n) cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        cache_cap = cap;
    }

    /* Fill missing values */
    for (idx_t i = cached_n + 1; i <= n; ++i) {      /* i is 1‑based index */
        if (i <= 13) {
            cache[i-1] = init_vals[i-1];
        } else {
            idx_t idx = i - 1;                         /* zero‑based index in cache */
            seq_t pos = 0, neg = 0;

            pos += cache[idx-1] * (seq_t)3;
            pos += cache[idx-2] * (seq_t)15;
            pos += cache[idx-6] * (seq_t)73;
            pos += cache[idx-7] * (seq_t)15;
            pos += cache[idx-9] * (seq_t)4;
            pos += cache[idx-12] * (seq_t)8;

            neg += cache[idx-3] * (seq_t)11;
            neg += cache[idx-4] * (seq_t)64;
            neg += cache[idx-5] * (seq_t)10;
            neg += cache[idx-8] * (seq_t)7;
            neg += cache[idx-10] * (seq_t)30;
            neg += cache[idx-11] * (seq_t)9;

            /* a(n) = pos - neg   (mod 2^w). Negation is performed with two's complement. */
            cache[idx] = pos + (~neg + 1);
        }
    }
    cached_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, but return 0 */
    ensure_cache(n);
    return cache[n-1];
}
