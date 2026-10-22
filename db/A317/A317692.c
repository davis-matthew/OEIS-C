/*
 * A317692  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3,
 *          4, 5, 6, 7 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 0..1 arrays with upper left element 0 and
 *            no two king‑adjacent elements equal. Empirical recurrence (valid
 *            for n>19): a(n) = 5*a(n-1) - 6*a(n-2) + 16*a(n-3) - 60*a(n-4) +
 *            43*a(n-5) - 36*a(n-6) + 156*a(n-7) - 49*a(n-8) - 12*a(n-9) -
 *            91*a(n-10) + 28*a(n-11) - 94*a(n-12) - 121*a...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[19] = {
    (seq_t)3ULL,          /* a(1) */
    (seq_t)9ULL,          /* a(2) */
    (seq_t)14ULL,         /* a(3) */
    (seq_t)33ULL,         /* a(4) */
    (seq_t)92ULL,         /* a(5) */
    (seq_t)220ULL,        /* a(6) */
    (seq_t)593ULL,        /* a(7) */
    (seq_t)1787ULL,       /* a(8) */
    (seq_t)5401ULL,       /* a(9) */
    (seq_t)16954ULL,      /* a(10) */
    (seq_t)55199ULL,      /* a(11) */
    (seq_t)182686ULL,     /* a(12) */
    (seq_t)613262ULL,     /* a(13) */
    (seq_t)2081449ULL,    /* a(14) */
    (seq_t)7111645ULL,    /* a(15) */
    (seq_t)24412407ULL,   /* a(16) */
    (seq_t)84069938ULL,   /* a(17) */
    (seq_t)290115461ULL,  /* a(18) */
    (seq_t)1002577520ULL  /* a(19) */
};

static const int coeffs[18] = {
    5,   -6,  16,  -60,  43,  -36, 156,  -49,  -12,  -91,
    28,  -94, -121, 362,  -148, -28,  16
};

static seq_t *cache = NULL;   /* cache array storing a(1) .. a(k) */
static size_t cache_len = 0;  /* number of values currently stored (>=0) */
static size_t cache_cap = 0;  /* allocated capacity of the cache */

/* Ensure that cache contains values up to a(n).  Grow the array if
   necessary and compute the missing terms using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        delete[] cache;
#else
        seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }
    for (idx_t i = cache_len; i < n; ++i) {
        if (i < 19) {
            cache[i] = init_values[i];
        } else {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int j = 0; j < 18; ++j) {
                int c = coeffs[j];
                seq_t v = cache[(size_t)(i - 1 - j)];
                if (c > 0) {
                    sum_pos += (seq_t)c * v;
                } else {
                    sum_neg += (seq_t)(-c) * v;
                }
            }
            cache[i] = sum_pos - sum_neg;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
