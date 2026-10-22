/*
 * A302226  Number of 3 X n 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = a(n-1)+17*a(n-2)+12*a(n-3)-59*a(n-4)-83*a(n-5)+10*a(n-6)+87*a(n-7)
          +108*a(n-8)+22*a(n-9)-104*a(n-10)-71*a(n-11)+2*a(n-13)+a(n-14)+2*a(n-15)
   for n>17, with
   a(1)=0, a(2)=11, a(3)=43, a(4)=164, a(5)=760, a(6)=3425, a(7)=15569,
   a(8)=70323, a(9)=318438, a(10)=1441136, a(11)=6522237, a(12)=29519913,
   a(13)=133601253, a(14)=604673484, a(15)=2736676090, a(16)=12385962613,
   a(17)=56057588893 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization --------------------------------------- */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* size of cache array (1‑based) */

/* seed values, indices 1..17 ------------------------------------------- */
    static const seq_t seed[18] = {
        0ULL,                  /* dummy for index 0 */
        0ULL,                  /* a(1) */
        11ULL,                 /* a(2) */
        43ULL,                 /* a(3) */
        164ULL,                /* a(4) */
        760ULL,                /* a(5) */
        3425ULL,               /* a(6) */
        15569ULL,              /* a(7) */
        70323ULL,              /* a(8) */
        318438ULL,             /* a(9) */
        1441136ULL,            /* a(10) */
        6522237ULL,            /* a(11) */
        29519913ULL,           /* a(12) */
        133601253ULL,          /* a(13) */
        604673484ULL,          /* a(14) */
        2736676090ULL,         /* a(15) */
        12385962613ULL,        /* a(16) */
        56057588893ULL         /* a(17) */
    };

    /* first time: allocate cache for 1…17 -------------------------------- */
    if (cache == NULL) {
        cache_size = 18;                 /* indices 0…17 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* unlikely, but keep type safe */
        for (idx_t i = 1; i < cache_size; ++i)
            cache[i] = seed[i];
    }

    /* if requested n already computed ------------------------------------- */
    if (n < cache_size)
        return cache[n];

    /* grow cache to hold new index n -------------------------------------- */
    size_t new_size = (size_t)(n + 1) * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, new_size);
    if (tmp == NULL)
        return (seq_t)0;    /* allocation failure – return 0 as safe default */
    cache = tmp;

    /* compute all missing terms up to n ----------------------------------- */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t pos = cache[i-1]                                          
                    + (seq_t)17 * cache[i-2]
                    + (seq_t)12 * cache[i-3]
                    + (seq_t)10 * cache[i-6]
                    + (seq_t)87 * cache[i-7]
                    + (seq_t)108 * cache[i-8]
                    + (seq_t)22 * cache[i-9]
                    + (seq_t)2  * cache[i-13]
                    + cache[i-14]
                    + (seq_t)2  * cache[i-15];

        seq_t neg = (seq_t)59  * cache[i-4]
                  + (seq_t)83  * cache[i-5]
                  + (seq_t)104 * cache[i-10]
                  + (seq_t)71  * cache[i-11];

        cache[i] = pos - neg;   /* sequence values are non‑negative */
    }

    cache_size = n + 1;          /* update current cache extent */
    return cache[n];
}
