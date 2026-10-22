/*
 * A302467  Number of nX3 0..1 arrays with every element equal to 1, 2, 3 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+9*a(n-2)+15*a(n-3)-68*a(n-4)-132*a(n-5)
 *            +96*a(n-6)+136*a(n-7)-34*a(n-8)-276*a(n-9)
 *            +188*a(n-10)-8*a(n-11)+32*a(n-12)-16*a(n-13)+64*a(n-14) Initial
 *            values: a(1)=1, a(2)=14, a(3)=49, a(4)=250, a(5)=1147,
 *            a(6)=5486, a(7)=25599, a(8)=121626, a(9)=572657, a(10)=2702270,
 *            a(11)=12758927,...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* the sequence is defined for n >= 1 */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* largest n for which cache[n] is valid */
    static idx_t cache_cap = 0;      /* allocated size (>= cache_len+1) */

    /* initial 14 values */
    static const seq_t init[14] = {
        (seq_t)1ULL,
        (seq_t)14ULL,
        (seq_t)49ULL,
        (seq_t)250ULL,
        (seq_t)1147ULL,
        (seq_t)5486ULL,
        (seq_t)25599ULL,
        (seq_t)121626ULL,
        (seq_t)572657ULL,
        (seq_t)2702270ULL,
        (seq_t)12758927ULL,
        (seq_t)60200726ULL,
        (seq_t)284075917ULL,
        (seq_t)1340774674ULL
    };

    /* allocate/cache initialization */
    if (cache == NULL) {
        cache_cap = 15;  /* indices 0..14, we ignore 0 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;  /* allocation failed */
        for (idx_t i = 1; i <= 14; ++i)
            cache[i] = init[i - 1];
        cache_len = 14;
    }

    /* enlarge cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* yield result if already computed */
    if (n <= cache_len)
        return cache[n];

    /* compute values from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        pos += cache[i - 1] * (seq_t)3;
        pos += cache[i - 2] * (seq_t)9;
        pos += cache[i - 3] * (seq_t)15;
        neg += cache[i - 4] * (seq_t)68;
        neg += cache[i - 5] * (seq_t)132;
        pos += cache[i - 6] * (seq_t)96;
        pos += cache[i - 7] * (seq_t)136;
        neg += cache[i - 8] * (seq_t)34;
        neg += cache[i - 9] * (seq_t)276;
        pos += cache[i - 10] * (seq_t)188;
        neg += cache[i - 11] * (seq_t)8;
        pos += cache[i - 12] * (seq_t)32;
        neg += cache[i - 13] * (seq_t)16;
        pos += cache[i - 14] * (seq_t)64;

        cache[i] = pos - neg;   /* pos >= neg mathematically */
    }

    cache_len = n;
    return cache[n];
}
