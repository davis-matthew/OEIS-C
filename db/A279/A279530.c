/*
 * A279530  Number of nX2 0..2 arrays with no element equal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for A279530 * a(1) = 1, a(2) = 0, a(3) = 15, a(4) =
 *            222, a(5) = 2348, a(6) = 21302, * a(7) = 176125, a(8) = 1370378,
 *            a(9) = 10206549, a(10) = 73563740, * a(11) = 516783536, a(12) =
 *            3556359632, a(13) = 24063074811, * and for n > 13: * a(n) =
 *            18*a(n-1) -108*a(n-2) +183*a(n-3) +408*a(n-4)...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, free */

seq_t a(idx_t n)
{
    /* Initial values for indices 1..13 (index 0 unused) */
    static const seq_t init[14] = {
        0ULL,                    /* placeholder for index 0 */
        1ULL,                    /* a(1) */
        0ULL,                    /* a(2) */
        15ULL,                   /* a(3) */
        222ULL,                  /* a(4) */
        2348ULL,                 /* a(5) */
        21302ULL,                /* a(6) */
        176125ULL,               /* a(7) */
        1370378ULL,              /* a(8) */
        10206549ULL,             /* a(9) */
        73563740ULL,             /* a(10) */
        516783536ULL,            /* a(11) */
        3556359632ULL,           /* a(12) */
        24063074811ULL           /* a(13) */
    };

    /* Handle trivial case */
    if (n <= 13)
        return init[(size_t)n];

    /* Dynamically grow a cache of computed values */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* First allocation and initialisation up to index 13 */
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[14];
#else
        cache = (seq_t*)malloc(sizeof(seq_t) * 14);
#endif
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init[i];
        cache[0] = 0;   /* unused */
        max_cached = 13;
    }

    /* Expand the cache if needed */
    if (n > max_cached) {
        idx_t new_size = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
#else
        new_cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
#endif
        /* Copy existing values */
        for (idx_t i = 0; i <= max_cached; ++i)
            new_cache[i] = cache[i];

        /* Compute the remaining terms up to n */
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            seq_t pos =   (seq_t)18  * new_cache[k - 1]
                         + (seq_t)183 * new_cache[k - 3]
                         + (seq_t)408 * new_cache[k - 4]
                         + (seq_t)69  * new_cache[k - 6]
                         + (seq_t)2442* new_cache[k - 7]
                         + (seq_t)1452* new_cache[k -10]
                         + (seq_t)64  * new_cache[k -12];

            seq_t neg =   (seq_t)108 * new_cache[k - 2]
                         + (seq_t)1332* new_cache[k - 5]
                         + (seq_t)1632* new_cache[k - 8]
                         + (seq_t)1043* new_cache[k - 9]
                         + (seq_t)528 * new_cache[k -11];

            new_cache[k] = pos - neg;
        }

        /* Free the old cache */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        max_cached = n;
    }

    return cache[n];
}
