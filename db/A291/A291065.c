/*
 * A291065  Number of maximal irredundant sets in the 2n-crossed prism graph.
 * https://oeis.org/A291065
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)-2*a(n-2)+28*a(n-3)+a(n-4)-25*a(n-5)-65*a(n-6)-412*a(n-7)+821*a(n-8)+814*a(n-9)-1872*a(n-10)+8*a(n-11)+2488*a(n-12)-1184*a(n-13)-2816*a(n-14)+128*a(n-15)+1024*a(n-16)
 *            offset: a(2)=12
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[16] = {
    (seq_t)12ULL,          /* a(2) */
    (seq_t)124ULL,         /* a(3) */
    (seq_t)588ULL,         /* a(4) */
    (seq_t)2319ULL,        /* a(5) */
    (seq_t)11094ULL,       /* a(6) */
    (seq_t)52882ULL,       /* a(7) */
    (seq_t)255900ULL,      /* a(8) */
    (seq_t)1213693ULL,     /* a(9) */
    (seq_t)5681867ULL,     /* a(10) */
    (seq_t)26951995ULL,    /* a(11) */
    (seq_t)128276070ULL,   /* a(12) */
    (seq_t)607245760ULL,   /* a(13) */
    (seq_t)2873887210ULL,  /* a(14) */
    (seq_t)13621766889ULL, /* a(15) */
    (seq_t)64560977404ULL, /* a(16) */
    (seq_t)305887320978ULL/* a(17) */
};

/* dynamic cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* number of allocated slots */
static idx_t cache_max = 1;   /* largest index for which a(n) is stored */

/* allocate or enlarge the cache so that indices up to 'need' are valid */
static void ensure_cache_size(idx_t need)
{
    if (need < cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap <= need) new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();              /* out of memory */

    cache = new_cache;
    /* initialise new slots to 0 */
    for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/* compute a(n) for the 2n-crossed prism irredundant set sequence */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* undefined below offset */

    ensure_cache_size(n);

    /* return cached value if already computed */
    if (n <= cache_max) return cache[n];

    /* start filling from the first index not yet computed */
    idx_t start = cache_max + 1;
    if (start < 2) start = 2;

    for (idx_t i = start; i <= n; ++i) {
        if (i <= 17) {          /* use the known base values */
            cache[i] = base[i - 2];
            continue;
        }

        /* compute a(i) using the linear recurrence
           a(i) = sum_pos - sum_neg  where
           sum_pos = 4*a(i-1)+28*a(i-3)+a(i-4)+821*a(i-8)+814*a(i-9)+8*a(i-11)+2488*a(i-12)+128*a(i-15)+1024*a(i-16)
           sum_neg = 2*a(i-2)+25*a(i-5)+65*a(i-6)+412*a(i-7)+1872*a(i-10)+1184*a(i-13)+2816*a(i-14)
        */
        seq_t sum_pos = 0;
        sum_pos += cache[i-1] * (seq_t)4;
        sum_pos += cache[i-3] * (seq_t)28;
        sum_pos += cache[i-4] * (seq_t)1;
        sum_pos += cache[i-8] * (seq_t)821;
        sum_pos += cache[i-9] * (seq_t)814;
        sum_pos += cache[i-11] * (seq_t)8;
        sum_pos += cache[i-12] * (seq_t)2488;
        sum_pos += cache[i-15] * (seq_t)128;
        sum_pos += cache[i-16] * (seq_t)1024;

        seq_t sum_neg = 0;
        sum_neg += cache[i-2] * (seq_t)2;
        sum_neg += cache[i-5] * (seq_t)25;
        sum_neg += cache[i-6] * (seq_t)65;
        sum_neg += cache[i-7] * (seq_t)412;
        sum_neg += cache[i-10] * (seq_t)1872;
        sum_neg += cache[i-13] * (seq_t)1184;
        sum_neg += cache[i-14] * (seq_t)2816;

        cache[i] = sum_pos - sum_neg;    /* guaranteed non‑negative */
    }

    cache_max = n;
    return cache[n];
}
