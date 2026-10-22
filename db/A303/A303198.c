/*
 * A303198  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 3, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence of A303198 * a(n) =
 *            a(n-1)+a(n-2)+12*a(n-4)-6*a(n-5)-7*a(n-6)-3*a(n-7)-26*a(n-8) *
 *            +9*a(n-9)+14*a(n-10)+5*a(n-11)+18*a(n-12) *
 *            -5*a(n-13)-9*a(n-14)-2*a(n-15)-3*a(n-16)+a(n-17)+a(n-18) * for n
 *            > 19, with initial values a(1)..a(19) below
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* Initial terms a(1) .. a(19) */
static const seq_t init_terms[19] = {
    (seq_t)4ULL,        /* a(1) */
    (seq_t)6ULL,        /* a(2) */
    (seq_t)12ULL,       /* a(3) */
    (seq_t)23ULL,       /* a(4) */
    (seq_t)43ULL,       /* a(5) */
    (seq_t)105ULL,      /* a(6) */
    (seq_t)233ULL,      /* a(7) */
    (seq_t)479ULL,      /* a(8) */
    (seq_t)968ULL,      /* a(9) */
    (seq_t)2146ULL,     /* a(10) */
    (seq_t)4683ULL,     /* a(11) */
    (seq_t)9928ULL,     /* a(12) */
    (seq_t)20709ULL,    /* a(13) */
    (seq_t)44652ULL,    /* a(14) */
    (seq_t)96244ULL,    /* a(15) */
    (seq_t)205631ULL,   /* a(16) */
    (seq_t)435063ULL,   /* a(17) */
    (seq_t)930935ULL,   /* a(18) */
    (seq_t)1994977ULL   /* a(19) */
};

static seq_t *cache = NULL;      /* sequence cache */
static size_t cache_cap = 0;     /* allocated size */
static idx_t  cache_upto = 0;    /* highest index computed */

/* Ensure cache is large enough to hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but safe */

    if (n <= 19) return init_terms[n - 1];

    /* Ensure cache large enough */
    if (n > cache_upto) {
        ensure_cache(n);

        /* Initialize first 19 terms in cache if not done */
        if (cache_upto == 0) {
            cache[0] = (seq_t)0;          /* dummy for index 0 */
            for (idx_t i = 1; i <= 19; ++i)
                cache[i] = init_terms[i - 1];
            cache_upto = 19;
        }

        /* Extend computation from cache_upto+1 up to n */
        for (idx_t k = cache_upto + 1; k <= n; ++k) {
            seq_t pos = cache[k - 1]  /* a(n-1) */
                       + cache[k - 2]  /* a(n-2) */
                       + (seq_t)12 * cache[k - 4]   /* 12*a(n-4) */
                       + (seq_t)9  * cache[k - 9]   /* 9*a(n-9)  */
                       + (seq_t)14 * cache[k -10]   /* 14*a(n-10)*/
                       + (seq_t)5  * cache[k -11]   /* 5*a(n-11) */
                       + (seq_t)18 * cache[k -12]   /* 18*a(n-12)*/
                       + cache[k -17]              /* a(n-17)  */
                       + cache[k -18];             /* a(n-18)  */

            seq_t neg = (seq_t)6  * cache[k - 5]   /* 6*a(n-5)  */
                       + (seq_t)7  * cache[k - 6]   /* 7*a(n-6)  */
                       + (seq_t)3  * cache[k - 7]   /* 3*a(n-7)  */
                       + (seq_t)26 * cache[k - 8]   /* 26*a(n-8) */
                       + (seq_t)5  * cache[k -13]   /* 5*a(n-13)*/
                       + (seq_t)9  * cache[k -14]   /* 9*a(n-14)*/
                       + (seq_t)2  * cache[k -15]   /* 2*a(n-15)*/
                       + (seq_t)3  * cache[k -16];  /* 3*a(n-16)*/

            cache[k] = pos - neg;
        }
        cache_upto = n;
    }

    return cache[n];
}
