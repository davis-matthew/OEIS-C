/*
 * A303406  Number of nX3 0..1 arrays with every element equal to 1, 2, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)+7*a(n-2)+35*a(n-3)-36*a(n-4)-151*a(n-5)-97*a(n-6)+34*a(n-7)+258*a(n-8)-724*a(n-9)+256*a(n-10)+1584*a(n-11)-240*a(n-12)-512*a(n-13)-304*a(n-14)+1728*a(n-15)-1344*a(n-16)-1792*a(n-17)+1024*a(n-18);
 *            Initial values: a(1)=1, a(2)=7, a(3)=28, a(4)=119, a(5)=541,
 *            a(6)=2327, a(7)=10384, a...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static idx_t cache_max = 0;
    static seq_t *cache = NULL;
    static size_t cache_alloc = 0;

    /* Hard‑coded initial values (a(1) … a(18)) */
    static const seq_t init[18] = {
        (seq_t)1, (seq_t)7, (seq_t)28, (seq_t)119, (seq_t)541,
        (seq_t)2327, (seq_t)10384, (seq_t)47491, (seq_t)208616,
        (seq_t)938105, (seq_t)4229023, (seq_t)18855226, (seq_t)84609455,
        (seq_t)379973132, (seq_t)1700940358, (seq_t)7627893865,
        (seq_t)34215621269, (seq_t)153337948168
    };

    /* Return if in the initial range */
    if (n <= 18)
        return init[n - 1];

    /* Initialise cache once */
    if (cache == NULL) {
        cache_alloc = 64;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failed */
        for (size_t i = 0; i < 18; ++i) cache[i] = init[i];
        cache_max = 18;
    }

    /* Grow cache if needed */
    if ((size_t)n > cache_alloc) {
        size_t nb = cache_alloc ? cache_alloc : 16;
        while (nb < (size_t)n) nb <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, nb * sizeof(seq_t));
        if (!tmp) return 0;                 /* reallocation failed */
        cache = tmp;
        cache_alloc = nb;
    }

    /* Return cached value if already computed */
    if (n <= cache_max)
        return cache[n - 1];

    /* Recurrence coefficients */
    static const long long coeff[18] = {
        2, 7, 35, -36, -151, -97, 34, 258,
        -724, 256, 1584, -240, -512, -304,
        1728, -1344, -1792, 1024
    };

    /* Compute terms from cache_max+1 up to n */
    for (idx_t idx = cache_max + 1; idx <= n; ++idx) {
        seq_t pos = 0;   /* sum of positive contributions */
        seq_t neg = 0;   /* sum of negative contributions */

        for (int j = 1; j <= 18; ++j) {
            seq_t term = cache[(size_t)(idx - j - 1)];
            long long c = coeff[j - 1];
            if (c >= 0)
                pos += term * (seq_t)c;
            else
                neg += term * (seq_t)(-c);
        }
        cache[(size_t)(idx - 1)] = pos - neg;   /* final value */
    }

    cache_max = n;
    return cache[n - 1];
}
