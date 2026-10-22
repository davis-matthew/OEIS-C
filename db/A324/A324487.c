/*
 * A324487  a(n) = A001350(n)^3.
 * https://oeis.org/A324487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001350(n)^3 . Recurrence (n>11): * a(n) = 4*a(n-1) +
 *            12*a(n-2) - 44*a(n-3) - 44*a(n-4) + 132*a(n-5) * + 66*a(n-6) -
 *            132*a(n-7) - 44*a(n-8) + 44*a(n-9) + 12*a(n-10) * - 4*a(n-11) -
 *            a(n-12) * Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=64,
 *            a(4)=125, * a(5)=1331, a(6)=4096, a(7)=24389, a(8)...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t mul_word(seq_t a, unsigned int b)
{
    /* Prevent overflow when seq_t is 64‑bit unsigned */
    if (sizeof(seq_t) <= 8)
        return (seq_t)((unsigned __int128)a * b);
    return a * b;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of elements stored */
    static size_t cache_cap = 0; /* allocated capacity */

    /* initial 12 values for indices 0‑11 */
    static const seq_t init[12] = {
        (seq_t)0,  (seq_t)1,  (seq_t)1,  (seq_t)64,
        (seq_t)125, (seq_t)1331, (seq_t)4096, (seq_t)24389,
        (seq_t)91125, (seq_t)438976, (seq_t)1771561, (seq_t)7880599
    };

    if (n < 12)
        return init[n];

    /* allocate and initialise cache if necessary */
    size_t need = (size_t)n + 1U;
    if (!cache) {
        cache_cap = (need > 64 ? need : 64);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;        /* allocation failure: return zero */
        for (size_t i = 0; i < 12 && i < cache_cap; ++i)
            cache[i] = init[i];
        cache_len = 12;
    } else if (cache_cap < need) {
        size_t newcap = cache_cap * 2;
        if (newcap < need)
            newcap = need;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;        /* allocation failure */
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t v = mul_word(cache[i-1], 4)
                + mul_word(cache[i-2], 12)
                - mul_word(cache[i-3], 44)
                - mul_word(cache[i-4], 44)
                + mul_word(cache[i-5], 132)
                + mul_word(cache[i-6], 66)
                - mul_word(cache[i-7], 132)
                - mul_word(cache[i-8], 44)
                + mul_word(cache[i-9], 44)
                + mul_word(cache[i-10], 12)
                - mul_word(cache[i-11], 4)
                - cache[i-12];

        cache[i] = v;
    }
    cache_len = need;
    return cache[n];
}
