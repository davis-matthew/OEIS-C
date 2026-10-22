/*
 * A370092  a(0) = 1, a(n) = (-1)^n + (1/2) * Sum_{j=1..n} (1-(-1)^j-(-2)^j) *
 *          binomial(n,j) * a(n-j) for n > 0.
 * https://oeis.org/A370092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(n) = (-1)^n + (1/2) * Sum_{j=1..n}
 *            (1-(-1)^j-(-2)^j) * * binomial(n,j) * a(n-j) * (n > 0) * * The
 *            function below computes the sequence by dynamic programming *
 *            with memoisation. It only relies on the standard C / C++
 *            operators * on seq_t and an external realloc, malloc and free
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* The sequence values and indices are defined by the build system. */
extern seq_t    seq_alloc_new(size_t);
extern seq_t    seq_alloc(size_t);
extern void     seq_free(seq_t *);

/* --------------------------------------------------------------------- */
/* Memoised computation of a(n)                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache   = NULL;          /* array of a[0..cache_cap-1]   */
    static size_t cache_cap  = 0;          /* allocated capacity           */
    static idx_t  cache_len  = 0;          /* number of terms already filled */

    /* Helper to grow the cache if necessary */
    if (cache == NULL) {
        /* first initialisation */
        cache_cap  = (n >= 1000) ? n + 1 : 1000;  /* start with a reasonable size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();           /* allocation failed */
        cache[0] = (seq_t)1;                  /* a(0) = 1           */
        cache_len = 1;
    }

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if they are not yet available */
    while (cache_len <= n) {
        idx_t i = cache_len;                  /* index of term to compute */

        /* accumulate the two parts of the sum separately   */
        seq_t sum_pos = 0;                    /* contribution from odd j   */
        seq_t sum_neg = 0;                    /* contribution from even j  */

        seq_t pow2  = 1;                      /* 2^0 initially            */
        seq_t binom = 1;                      /* C(i,0)                   */

        for (idx_t j = 1; j <= i; ++j) {
            /* update 2^j and C(i,j)   */
            pow2 = pow2 * 2;
            binom = binom * (seq_t)(i - j + 1) / (seq_t)j;

            /* product of binomial and the previously computed term */
            seq_t prod = binom * cache[i - j];

            if (j & 1) {                       /* j odd   */
                seq_t term = (seq_t)2 + pow2;  /* 2 + 2^j */
                prod = prod * term;
                sum_pos += prod;
            } else {                           /* j even  */
                prod = prod * pow2;           /* -2^j    (handled as subtraction later) */
                sum_neg += prod;
            }
        }

        /* total sum is even, so division by 2 is safe by shift */
        seq_t total = sum_pos - sum_neg;
        seq_t val   = total >> 1;               /* (1/2)*sum */

        /* add (-1)^i term */
        if ((i & 1) == 0) {
            val += (seq_t)1;                   /* +1 for i even */
        } else {
            val -= (seq_t)1;                   /* -1 for i odd   */
        }

        cache[i] = val;
        ++cache_len;
    }

    return cache[n];
}
