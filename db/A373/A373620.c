/*
 * A373620  Expansion of e.g.f. exp(x / (1 - x^2)^2).
 * https://oeis.org/A373620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=1, a(2)=1, a(3)=13, a(4)=49, a(5)=481, a(6)=3841
   a(n) = a(n-1)
          + 3*(n-1)*(n-2)*a(n-2)
          + 3*(n-1)*(n-2)*a(n-3)
          - 3*(n-1)*(n-2)*(n-3)*(n-4)*a(n-4)
          + (n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6)*a(n-6)   (n ≥ 6) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed = 0; /* highest index computed */

    /* Initialise cache with base values if first call */
    if (!cache) {
        const size_t init_size = 7;          /* indices 0..6 */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failure */
        cache_size = init_size;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)49;
        cache[5] = (seq_t)481;
        cache[6] = (seq_t)3841;
        computed = 6;
    }

    if (n <= computed)
        return cache[n];

    /* Grow cache if required */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_size = needed;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];

        seq_t i1 = (seq_t)(i - 1);
        seq_t i2 = (seq_t)(i - 2);
        seq_t prod12 = i1;
        prod12 *= i2;                       /* (i-1)*(i-2) */

        /* term: 3*(i-1)*(i-2)*a[i-2] */
        seq_t t2 = cache[i - 2];
        t2 *= prod12;
        t2 *= (seq_t)3;
        val += t2;

        /* term: 3*(i-1)*(i-2)*a[i-3] */
        seq_t t3 = cache[i - 3];
        t3 *= prod12;
        t3 *= (seq_t)3;
        val += t3;

        /* term: -3*(i-1)*(i-2)*(i-3)*(i-4)*a[i-4] */
        seq_t prod4 = prod12;
        prod4 *= (seq_t)(i - 3);
        prod4 *= (seq_t)(i - 4);
        seq_t t4 = cache[i - 4];
        t4 *= prod4;
        t4 *= (seq_t)3;
        val -= t4;

        /* term: +(i-1)*(i-2)*(i-3)*(i-4)*(i-5)*(i-6)*a[i-6] */
        seq_t prod6 = prod12;
        prod6 *= (seq_t)(i - 3);
        prod6 *= (seq_t)(i - 4);
        prod6 *= (seq_t)(i - 5);
        prod6 *= (seq_t)(i - 6);
        seq_t t6 = cache[i - 6];
        t6 *= prod6;
        val += t6;

        cache[i] = val;
    }

    computed = n;
    return cache[n];
}
