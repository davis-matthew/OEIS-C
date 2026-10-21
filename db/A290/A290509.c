/*
 * A290509  Number of irredundant sets in the n-Moebius ladder.
 * https://oeis.org/A290509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 2*a(n-4) - 6*a(n-5) - 8*a(n-6) +
 *            13*a(n-7) + 6*a(n-8) - 6*a(n-9) - 2*a(n-10) - 5*a(n-11) -
 *            4*a(n-12) + 2*a(n-13) + 2*a(n-14) - a(n-15) + a(n-16) +
 *            3*a(n-17) + 2*a(n-19) for n > 19, with initial values a(1)=3,
 *            a(2)=5, …, a(19)=460420182
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A290509.
 * This implementation uses the empirical linear recurrence above.
 */
#include <stddef.h>
#include <stdlib.h>

static const seq_t init_values[20] = {
    /* a(0) unused */
    0ULL,     /* 0 */
    3ULL,     /* 1 */
    5ULL,     /* 2 */
    24ULL,    /* 3 */
    81ULL,    /* 4 */
    198ULL,   /* 5 */
    542ULL,   /* 6 */
    1550ULL,  /* 7 */
    4497ULL,  /* 8 */
    12732ULL, /* 9 */
    36210ULL, /* 10 */
    103579ULL,/* 11 */
    296250ULL,/* 12 */
    846303ULL,/* 13 */
    2417308ULL,/* 14 */
    6907329ULL,/* 15 */
    19737889ULL,/* 16 */
    56396602ULL,/* 17 */
    161138306ULL,/* 18 */
    460420182ULL /* 19 */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* Ensure cache large enough to hold index n */
    size_t need = (size_t)n + 1;      /* +1 so that we can index n directly */
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) { /* allocation failed; caller must handle */ }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize cache[0] */
    if (max_computed == 0) {
        cache[0] = 0;
        max_computed = 0;
    }

    /* Compute values up to n lazily */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 19) {
            val = init_values[i];
        } else {
            /* Separate positive and negative contributions to avoid underflow */
            seq_t pos = 0;
            pos += cache[i - 1] * 3;
            pos += cache[i - 4] * 2;
            pos += cache[i - 7] * 13;
            pos += cache[i - 8] * 6;
            pos += cache[i - 13] * 2;
            pos += cache[i - 14] * 2;
            pos += cache[i - 16];
            pos += cache[i - 17] * 3;
            pos += cache[i - 19] * 2;

            seq_t neg = 0;
            neg += cache[i - 3];
            neg += cache[i - 5] * 6;
            neg += cache[i - 6] * 8;
            neg += cache[i - 9] * 6;
            neg += cache[i - 10] * 2;
            neg += cache[i - 11] * 5;
            neg += cache[i - 12] * 4;
            neg += cache[i - 15];

            val = pos - neg;
        }
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
