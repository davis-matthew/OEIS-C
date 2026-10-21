/*
 * A262334  Number of (n+3)X(2+3) 0..1 arrays with each row and column
 *          divisible by 9, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-1) -8*a(n-2) -8*a(n-3) +48*a(n-4) -64*a(n-5) +a(n-6)
 *             -6*a(n-7) +8*a(n-8) +8*a(n-9) -48*a(n-10) +64*a(n-11)
 * (empirical, valid for n >= 12, base values given for n = 1…11)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values a(1)..a(11) */
    static const seq_t base[12] = {
        0,          /* unused index 0 */
        4,          /* a(1) */
        16,         /* a(2) */
        64,         /* a(3) */
        225,        /* a(4) */
        841,        /* a(5) */
        3249,       /* a(6) */
        12996,      /* a(7) */
        51984,      /* a(8) */
        207936,     /* a(9) */
        829921,     /* a(10) */
        3316041     /* a(11) */
    };

    if (n == 0) return 0;          /* undefined for n<1, return 0 */

    /* Return base value for n <= 11 */
    if (n <= 11) return base[(size_t)n];

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache (number of elements) */
    static idx_t computed = 0; /* index up to which values are computed */

    /* Allocate / reallocate cache on first use or when more space needed */
    if (cache == NULL) {
        cap = 13;                    /* room for indices 0..12 at minimum */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed */
        /* store base values */
        for (size_t i = 1; i <= 11; ++i)
            cache[i] = base[i];
        computed = 11;
    }

    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* terms with positive coefficients */
        seq_t sumPos = 0;
        sumPos += 6   * cache[i - 1];
        sumPos += 48  * cache[i - 4];
        sumPos +=      cache[i - 6];
        sumPos += 8   * cache[i - 8];
        sumPos += 8   * cache[i - 9];
        sumPos += 64  * cache[i -11];

        /* terms with negative coefficients (subtracted later) */
        seq_t sumNeg = 0;
        sumNeg += 8   * cache[i - 2];
        sumNeg += 8   * cache[i - 3];
        sumNeg += 64  * cache[i - 5];
        sumNeg += 6   * cache[i - 7];
        sumNeg += 48  * cache[i -10];

        /* a(i) = sumPos - sumNeg;  sumPos >= sumNeg for all i */
        cache[i] = sumPos - sumNeg;
    }
    computed = n;
    return cache[n];
}
