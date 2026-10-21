/*
 * A267840  Number of symmetric difference-closed 4-sets consisting of the
 *          empty set and sets consisting of pairwise disjoint 2-subsets of
 *          {1,2,...,n}.
 * https://oeis.org/A267840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the recurrence (n-4)(n-2)*a(n) = 3(n²-5n+5)*a(n-1)
 *            + (n-1)(4n²-27n+41)*a(n-2) - (n-2)(n-1)(8n-29)*a(n-3) -
 *            (n-3)(n-2)(n-1)(3n-16)*a(n-4) + 3(n-4)(n-3)(n-2)(n-1)*a(n-5),
 *            with a(1)=0, a(2)=0, a(3)=0, a(4)=3, a(5)=15
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache   = NULL;   /* cache[0] unused */
static idx_t  max_computed = 0;     /* last index computed */

/* helper to initialize the cache when needed */
static void init_cache(void)
{
    if (!seq_cache) {
        seq_cache   = (seq_t*)calloc(6, sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        seq_cache[1] = (seq_t)0;
        seq_cache[2] = (seq_t)0;
        seq_cache[3] = (seq_t)0;
        seq_cache[4] = (seq_t)3;
        seq_cache[5] = (seq_t)15;
        max_computed = 5;
    }
}

seq_t a(idx_t n)
{
    /* initial terms up to n = 5 */
    static const seq_t init[6] = {0,0,0,0,3,15};
    if (n <= 5) return init[n];

    init_cache();

    /* grow cache if necessary */
    if (n + 1 > (idx_t)seq_cache[0]) { /* seq_cache[0] unused, so use allocation size read via malloc */
        size_t needed = n + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* initialize new elements to zero */
        for (idx_t j = max_computed + 1; j < needed; ++j) tmp[j] = 0;
        seq_cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
#if defined(__SIZEOF_INT128__)   /* use 128‑bit intermediate if available */
        __uint128_t num   = 0;
        __uint128_t denom = (__uint128_t)(i-4) * (i-2);

        __uint128_t coeff1 = 3 * ((__uint128_t)i * i - (__uint128_t)5 * i + 5);
        num += coeff1 * (__uint128_t)seq_cache[i-1];

        __uint128_t coeff2 = (__uint128_t)(i-1) *
             ((__uint128_t)4 * i * i - (__uint128_t)27 * i + 41);
        num += coeff2 * (__uint128_t)seq_cache[i-2];

        __uint128_t coeff3 = (__uint128_t)(i-2) * (i-1) * (8 * i - 29);
        num -= coeff3 * (__uint128_t)seq_cache[i-3];

        __uint128_t coeff4 = (__uint128_t)(i-3) * (i-2) * (i-1) * (3 * i - 16);
        num -= coeff4 * (__uint128_t)seq_cache[i-4];

        __uint128_t coeff5 = 3 * (__uint128_t)(i-4) * (i-3) * (i-2) * (i-1);
        num += coeff5 * (__uint128_t)seq_cache[i-5];

        seq_cache[i] = (seq_t)(num / denom);
#else   /* generic unsigned arithmetic (may overflow for very large n) */
        seq_t denom = (seq_t)(i-4) * (seq_t)(i-2);
        seq_t num   = 0;

        seq_t coeff1 = 3 * ((seq_t)i * i - (seq_t)5 * i + (seq_t)5);
        num += coeff1 * seq_cache[i-1];

        seq_t coeff2 = (seq_t)(i-1) *
            ((seq_t)4 * i * i - (seq_t)27 * i + (seq_t)41);
        num += coeff2 * seq_cache[i-2];

        seq_t coeff3 = (seq_t)(i-2) * (i-1) * (8 * i - 29);
        num -= coeff3 * seq_cache[i-3];

        seq_t coeff4 = (seq_t)(i-3) * (i-2) * (i-1) * (3 * i - 16);
        num -= coeff4 * seq_cache[i-4];

        seq_t coeff5 = 3 * (seq_t)(i-4) * (seq_t)(i-3) * (seq_t)(i-2) * (seq_t)(i-1);
        num += coeff5 * seq_cache[i-5];

        seq_cache[i] = num / denom;
#endif
    }

    max_computed = n;
    return seq_cache[n];
}
