/*
 * A306162  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2,
 *          5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A306162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A306162: Number of n×4 0/1 arrays with the top‑left element 0 and no
   equal king‑move neighbors.  The linear recurrence of order 18 (valid for
   n > 23) is
   a(n)=2a(n‑1)+4a(n‑2)-a(n‑4)-17a(n‑5)-22a(n‑6)+2a(n‑7)+38a(n‑8)+42a(n‑9)
   -5a(n‑10)-5a(n‑11)-9a(n‑12)-24a(n‑13)-26a(n‑14)-20a(n‑15)+6a(n‑16)+20a(n‑17)+16a(n‑18). */

#include <stdlib.h>

/* Initial 24 terms of the sequence (a(1)=8).  They are indexed from 1. */
static const seq_t base[24] = {
    8ULL, 9ULL, 32ULL, 103ULL, 298ULL, 962ULL,
    2966ULL, 8756ULL, 26287ULL, 79873ULL,
    241656ULL, 729008ULL, 2201471ULL, 6653034ULL,
    20101552ULL, 60722489ULL, 183440897ULL, 554200361ULL,
    1674289991ULL, 5058110361ULL, 15280857422ULL, 46164569525ULL,
    139466343261ULL, 421337009501ULL
};

/* Non‑zero coefficients of the recurrence and their offsets.  */
static const int coeff[18] = {
    2, 4, 0, -1, -17, -22, 2, 38, 42,
   -5, -5, -9, -24, -26, -20,  6, 20, 16
};

static const idx_t offset[18] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9,
   10, 11, 12, 13, 14, 15, 16, 17, 18
};

/* Helper: multiply a seq_t by a small positive integer using repeated addition. */
static seq_t mul_const(seq_t x, int mul) {
    seq_t res = 0;
    for (int i = 0; i < mul; ++i) {
        res += x;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Static cache for already computed values.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest n computed so far */
    static size_t cache_cap  = 0;   /* capacity allocated (indexable up to cache_cap-1) */

    /* Allocate initial cache if this is the first call.  */
    if (!cache) {
        cache_cap = 25; /* need indices 0..24 inclusive; index 0 unused */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();   /* allocation failure */
        for (idx_t i = 1; i <= 24; ++i) {
            cache[i] = base[i-1];
        }
        cache_size = 24;
    }

    /* Resize cache if required to hold up to index n. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return the value if it has already been computed. */
    if (n <= cache_size) return cache[n];

    /* Compute missing terms using the recurrence.  */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (int i = 0; i < 18; ++i) {
            int c = coeff[i];
            if (c == 0) continue;
            idx_t prev = k - offset[i];
            seq_t term = cache[prev];
            int mul = c > 0 ? c : -c;
            seq_t prod = mul_const(term, mul);
            if (c > 0) {
                sum += prod;
            } else {
                sum -= prod;
            }
        }
        cache[k] = sum;
    }
    cache_size = n;
    return cache[n];
}
