/*
 * A291536  Expansion of the series reversion of x*(1 + 4*x + x^2)/(1 - x)^4.
 * https://oeis.org/A291536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   A291536 = reversion of x*(1+4x+x^2)/(1-x)^4 Recurrence (n ≥ 5):
 *            7*n*(n-1)*(n+1)*a(n) +4*n*(n-1)*(58*n-83)*a(n-1)
 *            -36*(n-1)*(16*n^2-80*n+155)*a(n-2)
 *            +432*(-96*n^3+720*n^2-1794*n+1495)*a(n-3)
 *            +6912*(4*n-15)*(2*n-7)*(4*n-13)*a(n-4)=0 Initial values: a(1)=1,
 *            a(2)=-8, a(3)=101, a(4)=-1544. This functi...
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of stored terms */

    if (cache_size < 4) {
        cache = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)-8;         /* a(2) */
        cache[2] = (seq_t)101;        /* a(3) */
        cache[3] = (seq_t)-1544;      /* a(4) */
        cache_size = 4;
    }

    if (n <= cache_size) return cache[n-1];

    idx_t start = cache_size;            /* first missing term index (1‑based) */
    cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    cache_size = n;

    for (idx_t i = start; i < n; ++i) {
        idx_t N = i + 1;                /* term number a(N) */
        seq_t nn = (seq_t)N;
        seq_t a1 = cache[N-2];          /* a(N-1) */
        seq_t a2 = cache[N-3];          /* a(N-2) */
        seq_t a3 = cache[N-4];          /* a(N-3) */
        seq_t a4 = cache[N-5];          /* a(N-4) */

#ifdef __cplusplus
        /* C++ arbitrary‑precision build */
        seq_t c1 = 4 * nn * (nn - 1) * (58 * nn - 83);
        seq_t c2 = -36 * (nn - 1) * (16 * nn * nn - 80 * nn + 155);
        seq_t c3 = 432 * (-96 * nn * nn * nn + 720 * nn * nn - 1794 * nn + 1495);
        seq_t c4 = 6912 * (4 * nn - 15) * (2 * nn - 7) * (4 * nn - 13);
        seq_t denom = 7 * nn * (nn - 1) * (nn + 1);

        seq_t num = c1 * a1 + c2 * a2 + c3 * a3 + c4 * a4;
        cache[i] = -num / denom;
#else
        /* C 32/64/128‑bit build – use 128‑bit intermediates */
        __int128 t1 = (__int128)4 * nn * (nn - 1) * (58 * nn - 83);
        __int128 t2 = -(__int128)36 * (nn - 1)
                      * (16 * nn * nn - 80 * nn + 155);
        __int128 t3 = (__int128)432
                      * (-96 * nn * nn * nn + 720 * nn * nn - 1794 * nn + 1495);
        __int128 t4 = (__int128)6912 * (4 * nn - 15) * (2 * nn - 7) * (4 * nn - 13);
        __int128 denom128 = (__int128)7 * nn * (nn - 1) * (nn + 1);

        __int128 num = t1 * (__int128)a1 + t2 * (__int128)a2
                      + t3 * (__int128)a3 + t4 * (__int128)a4;

        cache[i] = -(seq_t)(num / denom128);
#endif
    }

    return cache[n-1];
}
