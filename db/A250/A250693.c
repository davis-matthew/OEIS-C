/*
 * A250693  Number of (2+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) -175*a(n-2) +882*a(n-3) -2835*a(n-4)
 *            +6072*a(n-5) -8777*a(n-6) +8458*a(n-7) -5204*a(n-8) +1848*a(n-9)
 *            -288*a(n-10) Initial values: a(1)=520, a(2)=2920, a(3)=13906,
 *            a(4)=60508, a(5)=249512, a(6)=995624, a(7)=3894542,
 *            a(8)=15061244, a(9)=57914756, a(10)=222291368
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static cache for computed terms */
static seq_t *cache = NULL;         /* array of a(k) values, 0‑based */
static idx_t cache_cap = 0;         /* allocated size */
static idx_t cache_len = 0;         /* highest index computed (0‑based) */

/* ensure cache has room for at least idx+1 elements */
static void cache_ensure(idx_t idx)
{
    if (idx < cache_cap) return;

    /* increase capacity exponentially */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= idx) new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);   /* abort if allocation fails */
    /* zero-initialize the newly allocated part for safety */
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) with 1‑based index */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    idx_t idx = n - 1;                 /* zero‑based index */

    /* if already computed, return */
    if (idx < cache_len) return cache[idx];

    /* initial values for n = 1..10 (indices 0..9) */
    static const seq_t init_vals[10] = {
        (seq_t)520ULL,
        (seq_t)2920ULL,
        (seq_t)13906ULL,
        (seq_t)60508ULL,
        (seq_t)249512ULL,
        (seq_t)995624ULL,
        (seq_t)3894542ULL,
        (seq_t)15061244ULL,
        (seq_t)57914756ULL,
        (seq_t)222291368ULL
    };

    /* coefficients grouped by sign to avoid negative multiplication */
    static const int coeff_pos[5] = {20, 882, 6072, 8458, 1848};
    static const int coeff_neg[5] = {175, 2835, 8777, 5204, 288};

    /* extend cache up to idx */
    cache_ensure(idx);

    /* compute missing terms up to idx */
    for (idx_t i = cache_len; i <= idx; ++i)
    {
        if (i < 10)
        {
            /* use initial values */
            cache[i] = init_vals[i];
        }
        else
        {
            /* sum of positive‑coefficient terms */
            seq_t sum_pos = 0;
            for (int j = 0; j < 5; ++j)
            {
                const seq_t *prev = &cache[i - (2 * j + 1)];
                seq_t prod = (*prev) * (seq_t)coeff_pos[j];
                sum_pos = sum_pos + prod;
            }

            /* sum of negative‑coefficient terms */
            seq_t sum_neg = 0;
            for (int j = 0; j < 5; ++j)
            {
                const seq_t *prev = &cache[i - (2 * j + 2)];
                seq_t prod = (*prev) * (seq_t)coeff_neg[j];
                sum_neg = sum_neg + prod;
            }

            /* a(i+1) = sum_pos – sum_neg  (positive result) */
            cache[i] = sum_pos - sum_neg;
        }
    }

    cache_len = idx + 1;   /* new highest index computed */

    return cache[idx];
}
