/*
 * A369940  Expansion of 1/(1 - x^3/(1-9*x)^(1/3)).
 * https://oeis.org/A369940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n>19:
 * (n-19)*(n-6)*(n-3)*a(n) =
 *   9*(3*n^3-88*n^2+675*n-1474)*a(n-1)
 *  -9*(27*n^3-828*n^2+7067*n-17838)*a(n-2)
 *   +81*(n-18)*(3*n-25)*(3*n-17)*a(n-3)
 *   +(n-19)*(n-6)*(n-3)*a(n-9)
 *   -18*(n^3-30*n^2+243*n-566)*a(n-10)
 *   +9*(n-18)*(3*n-25)*(3*n-17)*a(n-11)
 * Initial values (n=0..19) as given in the problem statement. */

#include <stdlib.h>

/* Allow extension for wider intermediate arithmetic when seq_t is 64‑bit */
#if defined(__SIZEOF_INT128__)
#define USE_INT128 1
#endif

#if defined(USE_INT128)
typedef unsigned __int128  big_t;
#else
typedef seq_t              big_t;
#endif

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Initial values for n = 0 .. 19 */
static const seq_t init_vals[20] = {
    (seq_t)1ULL,  (seq_t)0ULL,  (seq_t)0ULL,  (seq_t)1ULL,  (seq_t)3ULL,
    (seq_t)18ULL, (seq_t)127ULL, (seq_t)951ULL, (seq_t)7416ULL, (seq_t)59329ULL,
    (seq_t)483147ULL, (seq_t)3986415ULL, (seq_t)33224338ULL, (seq_t)279121233ULL,
    (seq_t)2360156580ULL, (seq_t)20063973502ULL, (seq_t)171337660872ULL,
    (seq_t)1468794800925ULL, (seq_t)12633200032942ULL, (seq_t)108974515627170ULL
};

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed) new_cap *= 2;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) exit(1);                /* OOM – caller can handle */
    cache = p;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    if (cache) return;
    ensure_capacity(20);
    for (size_t i = 0; i < 20; ++i) cache[i] = init_vals[i];
    cache_len = 20;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len) return cache[n];

    /* Grow cache to accommodate requested index */
    ensure_capacity(n + 1);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 20) {
            cache[i] = init_vals[i];
            continue;
        }

        big_t idx = (big_t)i;

        big_t term1 = (big_t)9 *
                      ( (big_t)3*idx*idx*idx - (big_t)88*idx*idx
                        + (big_t)675*idx - (big_t)1474 ) *
                      (big_t)cache[i-1];

        big_t term2 = (big_t)9 *
                      ( (big_t)27*idx*idx*idx - (big_t)828*idx*idx
                        + (big_t)7067*idx - (big_t)17838 ) *
                      (big_t)cache[i-2];

        big_t term3 = (big_t)81 *
                      (idx - (big_t)18) * ( (big_t)3*idx - (big_t)25 )
                      * ( (big_t)3*idx - (big_t)17 ) *
                      (big_t)cache[i-3];

        big_t term4 = (idx - (big_t)19) * (idx - (big_t)6) *
                      (idx - (big_t)3) *
                      (big_t)cache[i-9];

        big_t term5 = (big_t)18 *
                      ( idx*idx*idx - (big_t)30*idx*idx
                        + (big_t)243*idx - (big_t)566 ) *
                      (big_t)cache[i-10];

        big_t term6 = (big_t)9 *
                      (idx - (big_t)18) * ( (big_t)3*idx - (big_t)25 )
                      * ( (big_t)3*idx - (big_t)17 ) *
                      (big_t)cache[i-11];

        big_t num = term1 - term2 + term3 + term4 - term5 + term6;
        big_t den = (idx - (big_t)19) * (idx - (big_t)6) *
                    (idx - (big_t)3);

        cache[i] = (seq_t)(num / den);
    }

    cache_len = n + 1;
    return cache[n];
}
