/*
 * A364432  G.f. satisfies A(x) = 1 + x*A(x)*(2 + A(x)^3).
 * https://oeis.org/A364432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence of order 7
   3*n*(3*n-1)*(3*n+1)*a(n)
 +2*(-74*n^3 -375*n^2+ 665*n -252)*a(n-1)
 +12*(-337*n^3 +1941*n^2 -2984*n +1092)*a(n-2)
 +144*(-70*n^3 +861*n^2 -3347*n +4152)*a(n-3)
 +432*(n-4)*(31*n^2 -314*n +735)*a(n-4)
 -2592*(10*n-51) *(n-4)*(n-5)*a(n-5)
 +15552*(n-5)*(n-6) *(n-4)*a(n-6)=0
   initial values: a(0)=1, a(1)=3, a(2)=18, a(3)=162,
                    a(4)=1728, a(5)=20169, a(6)=249318.
   Computes a(n) for any n>=0. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* choose signed 128‑bit intermediate if available */
#if defined(__SIZEOF_INT128__)
typedef __int128 int128_t;
#else
typedef long long int128_t;
#endif

/* external linkage – required by the framework */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cache_size = 0; /* allocated size */
    static size_t computed = 0;   /* number of terms already computed */

    /* index as native unsigned integer */
    size_t idx = (size_t)n;

    /* lazy allocation and first 7 values */
    if (cache == NULL) {
        /* allocate space for the first 7 terms */
        cache_size = 8;                     /* a few extra slots */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)18;
        cache[3] = (seq_t)162;
        cache[4] = (seq_t)1728;
        cache[5] = (seq_t)20169;
        cache[6] = (seq_t)249318;

        computed = 7;                       /* terms 0..6 ready */
    }

    /* grow cache if needed */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* already computed? */
    if (idx < computed) return cache[idx];

    /* compute missing terms up to idx */
    for (size_t cur = computed; cur <= idx; ++cur) {
        idx_t i = (idx_t)cur;

        /* polynomial coefficients as signed 128‑bit */
        int128_t C0 = 3 * (__int128)i * (3 * (__int128)i - 1) *
                      (3 * (__int128)i + 1);
        int128_t C1 = 2 * ( -74 * (__int128)i * i * i
                           - 375 * (__int128)i * i
                           + 665 * (__int128)i
                           - 252 );
        int128_t C2 = 12 * (-337 * (__int128)i * i * i
                            + 1941 * (__int128)i * i
                            - 2984 * (__int128)i
                            + 1092 );
        int128_t C3 = 144 * (-70 * (__int128)i * i * i
                             + 861 * (__int128)i * i
                             - 3347 * (__int128)i
                             + 4152 );
        int128_t C4 = 432 * (i - 4) *
                      (31 * (__int128)i * i - 314 * (__int128)i + 735);
        int128_t C5 = -2592 * (10 * (__int128)i - 51) *
                      (i - 4) * (i - 5);
        int128_t C6 = 15552 * (i - 5) * (i - 6) * (i - 4);

        /* sum of previous terms * coefficients */
        int128_t sum = 0;
        sum += C1 * (__int128)cache[cur-1];
        sum += C2 * (__int128)cache[cur-2];
        sum += C3 * (__int128)cache[cur-3];
        sum += C4 * (__int128)cache[cur-4];
        sum += C5 * (__int128)cache[cur-5];
        sum += C6 * (__int128)cache[cur-6];

        /* solve for a(cur) */
        int128_t tmp = -sum / C0;
        cache[cur] = (seq_t)tmp;
    }

    computed = idx + 1;
    return cache[idx];
}
