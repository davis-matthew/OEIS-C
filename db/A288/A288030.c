/*
 * A288030  Number of minimal edge covers in the grid graph P_3 X P_n.
 * https://oeis.org/A288030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 3*a(n-1) + 6*a(n-2) + 35*a(n-3) - 7*a(n-4) * +
 *            15*a(n-5) - 55*a(n-6) + 31*a(n-7) + 36*a(n-8) * + 36*a(n-9) -
 *            124*a(n-10) - 8*a(n-12) + 32*a(n-13) * + a(n-14) + 9*a(n-15) -
 *            2*a(n-16) - 2*a(n-17) - a(n-18) * for n >= 18. * Initial values:
 *            * a(0)=1,a(1)=1,a(2)=6,a(3)=38,a(4)=190,a(5)=1021,...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* coefficients of the recurrence, coeff[0] corresponds to a(n-1) */
static const int coeff[18] = {
    3,   /* a(n-1)  */
    6,   /* a(n-2)  */
    35,  /* a(n-3)  */
    -7,  /* a(n-4)  */
    15,  /* a(n-5)  */
    -55, /* a(n-6)  */
    31,  /* a(n-7)  */
    36,  /* a(n-8)  */
    36,  /* a(n-9)  */
    -124,/* a(n-10)*/
    0,   /* a(n-11)*/
    -8,  /* a(n-12)*/
    32,  /* a(n-13)*/
    1,   /* a(n-14)*/
    9,   /* a(n-15)*/
    -2,  /* a(n-16)*/
    -2,  /* a(n-17)*/
    -1   /* a(n-18)*/
};

/* first 18 known values of the sequence */
static const seq_t init_vals[18] = {
    (seq_t)1ULL,
    (seq_t)1ULL,
    (seq_t)6ULL,
    (seq_t)38ULL,
    (seq_t)190ULL,
    (seq_t)1021ULL,
    (seq_t)5494ULL,
    (seq_t)29042ULL,
    (seq_t)154772ULL,
    (seq_t)824695ULL,
    (seq_t)4386942ULL,
    (seq_t)23356322ULL,
    (seq_t)124344111ULL,
    (seq_t)661873859ULL,
    (seq_t)3523418150ULL,
    (seq_t)18756407661ULL,
    (seq_t)99845472493ULL,
    (seq_t)531509598443ULL
};

/* dynamic cache of computed terms */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;   /* allocation size of cache */
static size_t cur_len   = 0;   /* number of terms currently computed,
                                 valid indices [0, cur_len) */

/* allocate and initialise cache with the first 18 terms */
static void ensure_cache(void)
{
    if (cache == NULL) {
        cache_cap = 18;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 18; ++i)
            cache[i] = init_vals[i];
        cur_len = 18;
    }
}

/* grow the cache to at least new_cap entries */
static void extend_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp)
        cache = tmp;
    cache_cap = new_cap;
}

/* compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    ensure_cache();

    if ((size_t)n < 18)
        return cache[(size_t)n];

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t cap = cache_cap;
        while (cap <= (size_t)n)
            cap <<= 1;           /* double until large enough */
        extend_cache(cap);
    }

    /* if n already computed, return it */
    if ((size_t)n < cur_len)
        return cache[(size_t)n];

    /* compute terms up to n (recurrence is valid for n >= 18) */
    for (size_t i = cur_len; i <= (size_t)n; ++i) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int j = 0; j < 18; ++j) {
            int c = coeff[j];
            if (c == 0) continue;
            seq_t val = cache[i - 1 - j];
            if (c > 0)
                pos_sum += val * (seq_t)c;
            else
                neg_sum += val * (seq_t)(-c);
        }
        cache[i] = pos_sum - neg_sum;   /* non‑negative by recurrence */
    }
    cur_len = (size_t)n + 1;

    return cache[(size_t)n];
}
