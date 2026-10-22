/*
 * A369839  Number of compositions of 5*n-4 into parts 1 and 5.
 * https://oeis.org/A369839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5),
 *            with a(1) = 1, a(2) = 3, a(3) = 11, a(4) = 45, a(5) = 185. This
 *            recurrence holds for all n ≥ 6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array indexed 1..computed */
    static size_t cap = 0;           /* allocated size (in elements) */
    static size_t computed = 0;      /* largest n already computed */

    if (n == 0) return 0; /* undefined for offset 1, but safe guard */

    /* grow cache if needed */
    if (n >= cap) {
        size_t newcap = cap ? cap << 1 : 64;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* initialise first five terms once */
    if (computed < 5) {
        /* ensure positions 1..5 are valid */
        if (cap < 6) {
            size_t newcap = 64;
            while (newcap < 6) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = newcap;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)45;
        cache[5] = (seq_t)185;
        computed = 5;
    }

    /* compute missing values up to n */
    for (size_t i = computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t t5 = cache[i-5];

        seq_t val = 6 * t1;
        val -= 10 * t2;
        val += 10 * t3;
        val -= 5 * t4;
        val += t5;

        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
