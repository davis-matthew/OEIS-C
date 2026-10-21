/*
 * A264342  Number of (2+1)X(n+1) arrays of permutations of 0..n*3+2 with each
 *          element having index change +-(.,.) 0,0 0,1 or 1,2.
 * https://oeis.org/A264342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            6*a(n-1)+26*a(n-2)-72*a(n-3)-195*a(n-4)+284*a(n-5)+555*a(n-6) *
 *            -424*a(n-7)-555*a(n-8)+284*a(n-9)+195*a(n-10)-72*a(n-11)-26*a(n-12)
 *            * +6*a(n-13)+a(n-14) * Initial values: a(1)=8, a(2)=55,
 *            a(3)=490, a(4)=3567, a(5)=28925, * a(6)=223356, a(7)=1759250,
 *            a(8)=13750304, a(9)=107777857, * a(10)=8...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined, but harmless */

    /* Initial 14 terms (fit in 64‑bit; cast to seq_t).          */
    static const seq_t init[14] = {
        8ULL,
        55ULL,
        490ULL,
        3567ULL,
        28925ULL,
        223356ULL,
        1759250ULL,
        13750304ULL,
        107777857ULL,
        843913133ULL,
        6610346674ULL,
        51772001513ULL,
        405494413096ULL,
        3175909815583ULL
    };

    /* Cache of computed terms: cache[0] = a(1), cache[cache_size-1] = a(cache_size). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of terms stored */

    /* Initialise cache with the first 14 terms. */
    if (cache == NULL) {
        cache_size = 14;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i) {
            cache[i] = init[i];
        }
    }

    /* Return cached value if already known. */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Grow cache to accommodate up to term n (1‑based). */
    idx_t new_size = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {                    /* realloc failure – fall back to direct computation. */
        /* Allocate a separate buffer if realloc fails. */
        tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;        /* out of memory: return 0. */
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
    }
    cache = tmp;

    /* Compute new terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t a1  = cache[i - 2];  /* a(i-1) */
        seq_t a2  = cache[i - 3];  /* a(i-2) */
        seq_t a3  = cache[i - 4];  /* a(i-3) */
        seq_t a4  = cache[i - 5];  /* a(i-4) */
        seq_t a5  = cache[i - 6];  /* a(i-5) */
        seq_t a6  = cache[i - 7];  /* a(i-6) */
        seq_t a7  = cache[i - 8];  /* a(i-7) */
        seq_t a8  = cache[i - 9];  /* a(i-8) */
        seq_t a9  = cache[i -10];  /* a(i-9) */
        seq_t a10 = cache[i -11];  /* a(i-10) */
        seq_t a11 = cache[i -12];  /* a(i-11) */
        seq_t a12 = cache[i -13];  /* a(i-12) */
        seq_t a13 = cache[i -14];  /* a(i-13) */
        seq_t a14 = cache[i -15];  /* a(i-14) */

        seq_t val = 6  *a1 + 26*a2 - 72*a3 - 195*a4
                    + 284*a5 + 555*a6 - 424*a7 - 555*a8
                    + 284*a9 + 195*a10 - 72*a11 - 26*a12
                    + 6*a13 + a14;

        cache[i - 1] = val;           /* store a(i) */
    }

    cache_size = n;                   /* update size after computation */
    return cache[n - 1];
}
