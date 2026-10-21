/*
 * A245162  Number of length n 0..7 arrays with new values introduced in order
 *          from both ends.
 * https://oeis.org/A245162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=9, a(6)=23, a(7)=65, *
 *            a(8)=199, a(9)=654, a(10)=2296, a(11)=8569, a(12)=33825, *
 *            a(13)=140581, a(14)=612933 * For n>14: * a(n) = 50*a(n-1)
 *            -1127*a(n-2) +15148*a(n-3) * -135303*a(n-4) +846930*a(n-5)
 *            -3816041*a(n-6) * +12508144*a(n-7) -29761816*a(n-8) +506475...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 31, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms, 1‑based */
static idx_t cache_len = 0;          /* number of terms stored (>=0) */
static idx_t cache_cap = 0;          /* allocated capacity (1‑based) */

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    /* known terms from a(1) to a(14) */
    const seq_t init_terms[15] = {
        0,          /* unused } cache[0] */
        (seq_t)1,   /* a(1) */
        (seq_t)1,   /* a(2) */
        (seq_t)2,   /* a(3) */
        (seq_t)4,   /* a(4) */
        (seq_t)9,   /* a(5) */
        (seq_t)23,  /* a(6) */
        (seq_t)65,  /* a(7) */
        (seq_t)199, /* a(8) */
        (seq_t)654, /* a(9) */
        (seq_t)2296,/* a(10) */
        (seq_t)8569,/* a(11) */
        (seq_t)33825,/* a(12) */
        (seq_t)140581,/* a(13) */
        (seq_t)612933/* a(14) */
    };

    cache_cap = 32;                     /* initial capacity (1‑based) */
    cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
    cache[0] = 0;                       /* sentinel */
    for (idx_t i = 1; i <= 14; ++i)
        cache[i] = init_terms[i];
    cache_len = 14;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined; return 0 for safety */

    if (cache == NULL)
        init_cache();

    if (n <= cache_len)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* Reallocate if necessary */
        ensure_capacity(i);

        if (i <= 14) {
            /* already initialized */
            continue;
        }

        /* compute a(i) by recurrence using only unsigned arithmetic */
        seq_t pos = (seq_t)50      * cache[i-1]
                  + (seq_t)15148   * cache[i-3]
                  + (seq_t)846930  * cache[i-5]
                  + (seq_t)12508144 * cache[i-7]
                  + (seq_t)50647520 * cache[i-9]
                  + (seq_t)46142208 * cache[i-11]
                  + (seq_t)4147200  * cache[i-13];

        seq_t neg = (seq_t)1127    * cache[i-2]
                  + (seq_t)135303   * cache[i-4]
                  + (seq_t)3816041  * cache[i-6]
                  + (seq_t)29761816 * cache[i-8]
                  + (seq_t)59753232 * cache[i-10]
                  + (seq_t)20839680 * cache[i-12];

        cache[i] = pos - neg; /* pos >= neg by definition of the recurrence */
    }

    cache_len = n;
    return cache[n];
}
