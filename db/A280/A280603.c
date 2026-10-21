/*
 * A280603  Number of nX7 0..1 arrays with no element equal to more than two
 *          of its horizontal, vertical and antidiagonal neighbors and with
 *          new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A280603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -8*a(n-2) +4*a(n-3) +2*a(n-8) -2*a(n-9)
 *            -3*a(n-10) +5*a(n-11) -a(n-12) -a(n-13) -a(n-16) +a(n-17), for n
 *            > 22. Initial values (a(1)..a(22)): 64, 1063, 1844, 3996, 8031,
 *            15792, 31152, 62349, 126893, 261994, 546766, 1149413, 2427843,
 *            5143554, 10917253, 23198800, 49332659, 104956473,...
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 22, u64 n <= 52, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The first 22 terms, indexed 0..21 correspond to a(1)..a(22) */
static const seq_t init_vals[22] = {
    64ULL, 1063ULL, 1844ULL, 3996ULL,
    8031ULL, 15792ULL, 31152ULL, 62349ULL,
    126893ULL, 261994ULL, 546766ULL, 1149413ULL,
    2427843ULL, 5143554ULL, 10917253ULL, 23198800ULL,
    49332659ULL, 104956473ULL, 223368832ULL, 475481120ULL,
    1012314087ULL, 2155517167ULL
};

/* Static cache for memoization */
static seq_t *cache = NULL;     /* cache[0] = a(1) */
static idx_t cache_len = 0;     /* number of terms currently stored */

/* Return a(n) for n >= 1, memoizing results */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* If the first 22 terms are requested, return from the table */
    if (n <= 22)
        return init_vals[n - 1];

    /* Initialize cache on first use */
    if (cache == NULL) {
        cache_len = 22;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        for (idx_t i = 0; i < 22; ++i)
            cache[i] = init_vals[i];
    }

    /* If the requested term is already cached, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache to hold terms up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    if (new_cache == NULL)   /* allocation failure – just return 0 */
        return 0;
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i is zero‑based index of a(i+1) */
        seq_t term =
            (seq_t)5 * cache[i - 1]          /* 5 * a(n-1) */
          - (seq_t)8 * cache[i - 2]          /* 8 * a(n-2) */
          + (seq_t)4 * cache[i - 3]          /* 4 * a(n-3) */
          + (seq_t)2 * cache[i - 8]          /* 2 * a(n-8) */
          - (seq_t)2 * cache[i - 9]          /* 2 * a(n-9) */
          - (seq_t)3 * cache[i -10]          /* 3 * a(n-10) */
          + (seq_t)5 * cache[i -11]          /* 5 * a(n-11) */
          -            cache[i -12]          /* a(n-12) */
          -            cache[i -13]          /* a(n-13) */
          -            cache[i -16]          /* a(n-16) */
          +            cache[i -17];         /* a(n-17) */
        cache[i] = term;
    }

    cache_len = n;
    return cache[n - 1];
}
