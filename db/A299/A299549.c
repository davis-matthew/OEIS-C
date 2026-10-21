/*
 * A299549  Number of nX3 0..1 arrays with every element equal to 2, 3, 4, 5,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2) -28*a(n-3) -27*a(n-4) +62*a(n-5)
 *            +16*a(n-6) -89*a(n-7) +49*a(n-8) +72*a(n-9) -56*a(n-10)
 *            +5*a(n-11) +10*a(n-12) -8*a(n-13) for n > 15 Initial values
 *            (1‑based indices): a(1)=0, a(2)=3, a(3)=9, a(4)=31, a(5)=166,
 *            a(6)=752, a(7)=3739, a(8)=18300, a(9)=90313, a(10)=446451,...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* size_t, NULL */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*, size_t, size_t) */

/* ---------- Initial values (1‑based) ---------- */
static const seq_t init_vals[15] = {
    /* 1  */ (seq_t)0,
    /* 2  */ (seq_t)3,
    /* 3  */ (seq_t)9,
    /* 4  */ (seq_t)31,
    /* 5  */ (seq_t)166,
    /* 6  */ (seq_t)752,
    /* 7  */ (seq_t)3739,
    /* 8  */ (seq_t)18300,
    /* 9  */ (seq_t)90313,
    /*10  */ (seq_t)446451,
    /*11  */ (seq_t)2207877,
    /*12  */ (seq_t)10926364,
    /*13  */ (seq_t)54077370,
    /*14  */ (seq_t)267676863,
    /*15  */ (seq_t)1325022222
};

/* ---------- Recurrence coefficients ---------- */
static const int coeff[13] = {
    5,  6, -28, -27, 62, 16, -89, 49, 72, -56, 5, 10, -8
};

/* ---------- Cache for computed terms ---------- */
static seq_t *cache = NULL;          /* array indexed 1… */
static size_t cache_cap = 0;         /* allocated capacity (index+1) */
static size_t cache_last = 0;        /* highest index that is already computed */

/* ---------- Ensure cache contains terms up to |n| ---------- */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    /* Lazily initialise the cache with the first 15 terms. */
    if (cache_cap == 0) {
        cache_cap = 16; /* indices 0..15, with index 0 unused */
        cache = seq_realloc(NULL, 0, cache_cap);
        if (!cache) { /* allocation failure – terminate early */
            cache_cap = cache_last = 0;
            return;
        }
        cache[0] = (seq_t)0; /* unused sentinel */
        for (size_t i = 1; i <= 15; ++i)
            cache[i] = init_vals[i - 1];
        cache_last = 15;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1; /* need indices 0…n */
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) { /* allocation failure – keep old cache */
            return;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
        if (i <= 15) { /* already initialised, but guard for safety */
            continue;
        }
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int k = 0; k < 13; ++k) {
            size_t idx = i - k - 1; /* a(i-k) */
            int c = coeff[k];
            if (c >= 0) {
                pos += cache[idx] * (seq_t)c;
            } else {
                neg += cache[idx] * (seq_t)(-c);
            }
        }
        cache[i] = pos - neg;
    }
    cache_last = (size_t)n;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined below offset, return 0 */
    ensure_cache(n);
    if (!cache) return (seq_t)0; /* allocation failure – give 0 */
    return cache[(size_t)n];
}
