/*
 * A296316  Number of n X 4 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 1 neighboring 1.
 * https://oeis.org/A296316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (order 21):
a(n) = a(n-1)+6*a(n-2)+29*a(n-3)+30*a(n-4)+61*a(n-5)+9*a(n-6)+73*a(n-7)
       -93*a(n-8)+134*a(n-9)-201*a(n-10)+173*a(n-11)-179*a(n-12)+107*a(n-13)
       -1*a(n-14)-30*a(n-15)-15*a(n-16)+22*a(n-17)-3*a(n-18)-2*a(n-19)-1*a(n-20)
       +1*a(n-21)
Initial values (n = 1 … 21):
4, 27, 96, 434, 1970, 8470, 37431, 164807, 723019, 3180521,
13980961, 61446863, 270126432, 1187392203, 5219407184, 22943336005,
100852689192, 443321760393, 1948727874003, 8566095221354, 37654312064817
*/

#include <stdint.h>
#include <stdlib.h>

static const seq_t init_vals[21] = {
    4ULL, 27ULL, 96ULL, 434ULL, 1970ULL, 8470ULL, 37431ULL,
    164807ULL, 723019ULL, 3180521ULL, 13980961ULL, 61446863ULL,
    270126432ULL, 1187392203ULL, 5219407184ULL, 22943336005ULL,
    100852689192ULL, 443321760393ULL, 1948727874003ULL,
    8566095221354ULL, 37654312064817ULL
};

static const int32_t COEFF[22] = {
    0,      /* unused, 1‑based */
    1,      /* a(n-1) */
    6,      /* a(n-2) */
    29,     /* a(n-3) */
    30,     /* a(n-4) */
    61,     /* a(n-5) */
    9,      /* a(n-6) */
    73,     /* a(n-7) */
    -93,    /* a(n-8) */
    134,    /* a(n-9) */
    -201,   /* a(n-10) */
    173,    /* a(n-11) */
    -179,   /* a(n-12) */
    107,    /* a(n-13) */
    -1,     /* a(n-14) */
    -30,    /* a(n-15) */
    -15,    /* a(n-16) */
    22,     /* a(n-17) */
    -3,     /* a(n-18) */
    -2,     /* a(n-19) */
    -1,     /* a(n-20) */
    1       /* a(n-21) */
};

static seq_t *cache = NULL;   /* dynamic array holding computed terms */
static idx_t cache_size = 0;  /* number of terms stored */
static idx_t cache_cap   = 0; /* allocated capacity */

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise cache with the first 21 known terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;                      /* already initialised */
    ensure_capacity(32);              /* enough capacity for 21 terms */
    for (idx_t i = 0; i < 21; ++i)
        cache[i] = init_vals[i];
    cache_size = 21;
}

/* Generate terms up to at least index 'n' (1‑based) */
static void compute_terms(idx_t n)
{
    init_cache();
    while (cache_size < n) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 1; k <= 21; ++k) {
            int32_t c = COEFF[k];
            if (!c)
                continue;
            seq_t prod = cache[cache_size - k] *
                         (seq_t)(c > 0 ? (uint64_t)c : (uint64_t)(-c));
            if (c > 0)
                pos_sum += prod;
            else
                neg_sum += prod;
        }
        /* Final value is pos_sum - neg_sum; pos_sum ≥ neg_sum by definition */
        seq_t next = pos_sum - neg_sum;
        ensure_capacity(cache_size + 1);
        cache[cache_size] = next;
        ++cache_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1, but guard against 0 */
        return (seq_t)0;
    compute_terms(n);
    return cache[n - 1];
}
