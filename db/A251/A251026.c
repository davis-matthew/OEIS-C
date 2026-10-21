/*
 * A251026  Number of (n+1)X(6+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 25*a(n-2) + 35*a(n-3) - 7*a(n-4) - 49*a(n-5) +
 *            77*a(n-6) - 55*a(n-7) + 20*a(n-8) - 3*a(n-9) for n>14 with
 *            a(1)=49481, a(2)=144737, a(3)=272547, a(4)=563271, a(5)=1078862,
 *            a(6)=2160510, a(7)=4230166, a(8)=8587303, a(9)=17595371,
 *            a(10)=37245814, a(11)=80267686, a(12)=176914157, a(...
 * Range:     u8 none, u16 n <= 1, u32 n <= 15, u64 n <= 42, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* The first 14 terms, needed for the recurrence seed.  */
static const seq_t A251026_start[14] = {
    49481ULL, 144737ULL, 272547ULL, 563271ULL,
    1078862ULL, 2160510ULL, 4230166ULL, 8587303ULL,
    17595371ULL, 37245814ULL, 80267686ULL,
    176914157ULL, 395061425ULL, 892649426ULL
};

/* cache of already computed values: cache[0] = a(1), … */
static seq_t *cache = NULL;      /* dynamic array of seq_t */
static size_t cache_cap = 0;     /* allocated capacity in elements */
static size_t cache_len = 0;     /* number of values actually stored */

/* Euclidean recurrence coefficients (a(n-1) … a(n-9)).  */
static const int coeff[9] = {
     8,   -25,  35,   -7,  -49,
    77,  -55,  20,   -3
};

/* Extend the cache array to hold at least 'need' elements.  */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* out‑of‑memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    if (cache_len) return;          /* already initialised */
    ensure_cache(14);
    for (size_t i = 0; i < 14; ++i) cache[i] = A251026_start[i];
    cache_len = 14;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used for OEIS offset, but safe */
    init_cache();

    if ((size_t)n <= cache_len)        /* already computed */
        return cache[n - 1];

    /* compute new terms up to n, extending the cache */
    ensure_cache((size_t)n);
    for (size_t m = cache_len + 1; m <= (size_t)n; ++m) {
        seq_t sum = 0;
        /* recurrence uses a(m-1) … a(m-9) */
        for (int i = 1; i <= 9; ++i) {
            /* index in cache: a(m-i) = cache[(m-i)-1] */
            seq_t term = cache[(m - i) - 1];
            sum += (seq_t)coeff[i - 1] * term;
        }
        cache[cache_len] = sum;
        cache_len++;
    }
    return cache[n - 1];
}
