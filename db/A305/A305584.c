/*
 * A305584  Number of nX6 0..1 arrays with every element unequal to 0, 1, 2, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(n) = 2*a(n-1)+5*a(n-2)+33*a(n-3)
 *                 -14*a(n-4)-54*a(n-5)-238*a(n-6)
 *                 +86*a(n-7)+54*a(n-8)+467*a(n-9)
 *                 -144*a(n-10)-48*a(n-11)-378*a(n-12)
 *                 +144*a(n-13)+324*a(n-15)   for n>20
 * Initial terms (n = 1..20):
 *   32, 13, 33, 102, 327, 1897, 8182, 33142, 152197, 674090,
 *   2919193, 12923734, 57045467, 250441014, 1103213496,
 *   4860506415, 21390637972, 94180079216, 414722020105,
 *   1825861346649
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 20 terms stored as constants */
    static const seq_t init[20] = {
        (seq_t)32, (seq_t)13, (seq_t)33, (seq_t)102, (seq_t)327,
        (seq_t)1897, (seq_t)8182, (seq_t)33142, (seq_t)152197,
        (seq_t)674090, (seq_t)2919193, (seq_t)12923734,
        (seq_t)57045467, (seq_t)250441014, (seq_t)1103213496,
        (seq_t)4860506415, (seq_t)21390637972, (seq_t)94180079216,
        (seq_t)414722020105, (seq_t)1825861346649
    };

    if (n == 0)
        return (seq_t)0;               /* undefined below offset, safe guard */

    if (n <= 20)
        return init[n-1];

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0, cache_len = 0;

    /* initialise cache with first 20 values if needed */
    if (cache == NULL) {
        cache_cap = 20;
        cache_len = 20;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure */
        for (idx_t i = 0; i < 20; ++i)
            cache[i] = init[i];
    }

    /* expand cache capacity if required */
    if (n > cache_cap) {
        idx_t new_cap = n * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n, if not already present */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i < 20) {
            /* already stored in init, safety copy */
            cache[i] = init[i];
            continue;
        }

        /* compute using the recurrence, avoiding unsigned underflow */
        seq_t pos = 0, neg = 0;

        pos += cache[i-1] * (seq_t)2;
        pos += cache[i-2] * (seq_t)5;
        pos += cache[i-3] * (seq_t)33;
        pos += cache[i-7] * (seq_t)86;
        pos += cache[i-8] * (seq_t)54;
        pos += cache[i-9] * (seq_t)467;
        pos += cache[i-13] * (seq_t)144;
        pos += cache[i-15] * (seq_t)324;

        neg += cache[i-4]  * (seq_t)14;
        neg += cache[i-5]  * (seq_t)54;
        neg += cache[i-6]  * (seq_t)238;
        neg += cache[i-10] * (seq_t)144;
        neg += cache[i-11] * (seq_t)48;
        neg += cache[i-12] * (seq_t)378;

        cache[i] = pos - neg;
    }

    cache_len = n;
    return cache[n-1];
}
