/*
 * A250789  Number of (7+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 21, u64 n <= 53, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A250789: Number of (7+1) X (n+1) 0..1 arrays with
   nondecreasing x(i,j)+x(i,j-1) in the i direction and
   nondecreasing |x(i,j)-x(i-1,j)| in the j direction.

   Recurrence (valid for n ≥ 11):
     a(n) = 8*a(n-1) - 25*a(n-2) + 35*a(n-3) - 9*a(n-4)
            - 37*a(n-5) + 47*a(n-6) - 15*a(n-7) - 10*a(n-8)
            + 9*a(n-9)  -  2*a(n-10)

   Initial values (n = 1 … 10):
     576, 1596, 4054, 10306, 25126, 59590, 137082,
     307762, 676266, 1460260
   All computations use type seq_t; idx_t is uint64_t. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* static memoization cache */
    static seq_t *cache  = NULL;
    static size_t cache_size = 0;   /* number of terms stored (≥0) */
    static size_t cache_cap  = 0;   /* allocated slots           */

    /* initialise cache storage on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* --- load the 10 known initial terms if not yet present --- */
    if (cache_size < 10) {
        static const seq_t init[10] = {576ULL, 1596ULL, 4054ULL, 10306ULL,
                                       25126ULL, 59590ULL, 137082ULL,
                                       307762ULL, 676266ULL, 1460260ULL};
        size_t need = 10 - cache_size;

        /* grow cache if necessary */
        if (cache_cap < cache_size + need) {
            size_t new_cap = cache_cap;
            while (new_cap < cache_size + need) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* copy remaining initial terms */
        for (size_t i = cache_size; i < 10; ++i)
            cache[cache_size++] = init[i];
    }

    /* return value if already computed */
    if (n <= cache_size) return cache[n - 1];

    /* --- compute terms up to n using the recurrence --- */
    for (idx_t cur = cache_size + 1; cur <= n; ++cur) {
        seq_t pos = 0, neg = 0;

        pos += cache[cur - 2] * (seq_t)8;   /* 8*a(cur-1)   */
        neg += cache[cur - 3] * (seq_t)25;  /* 25*a(cur-2)  */
        pos += cache[cur - 4] * (seq_t)35;  /* 35*a(cur-3)  */
        neg += cache[cur - 5] * (seq_t)9;   /* 9*a(cur-4)   */
        neg += cache[cur - 6] * (seq_t)37;  /* 37*a(cur-5)  */
        pos += cache[cur - 7] * (seq_t)47;  /* 47*a(cur-6)  */
        neg += cache[cur - 8] * (seq_t)15;  /* 15*a(cur-7)  */
        neg += cache[cur - 9] * (seq_t)10;  /* 10*a(cur-8)  */
        pos += cache[cur -10] * (seq_t)9;   /* 9*a(cur-9)   */
        neg += cache[cur -11] * (seq_t)2;   /* 2*a(cur-10)  */

        seq_t val = pos - neg;   /* result is non‑negative */

        /* grow cache if needed */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
