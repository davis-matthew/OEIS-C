/*
 * A345007  a(0) = 1; a(3*n) = a(n) - a(n-1), a(3*n+1) = a(3*n+2) = -a(n).
 * https://oeis.org/A345007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(3*n)=a(n)-a(n-1); a(3*n+1)=a(3*n+2)=-a(n)
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Compute term a(n) of OEIS A345007 */
seq_t a(idx_t n)
{
    /* static cache structures ------------------------------------------------ */
    static seq_t *cache     = NULL;          /* computed values */
    static unsigned char *computed = NULL;   /* 1 if value known, else 0 */
    static size_t cache_cap = 0;             /* current capacity of cache */

    /* Ensure cache can hold index n ------------------------------------------ */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        unsigned char *tmp_computed =
            (unsigned char *)realloc(computed, new_cap * sizeof(unsigned char));
        cache = tmp_cache;
        computed = tmp_computed;
        /* initialise new slots as unknown */
        memset(computed + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(unsigned char));
        cache_cap = new_cap;
    }

    /* Return cached value if available ---------------------------------------- */
    if (computed[(size_t)n]) {
        return cache[(size_t)n];
    }

    /* Compute the value -------------------------------------------------------- */
    seq_t result;
    if (n == 0) {
        result = 1;
    } else if (n % 3 == 0) {
        idx_t k = n / 3;
        seq_t ak   = a(k);
        seq_t akm1 = a(k - 1);  /* k >= 1 because n >= 3 */
        result = ak - akm1;
    } else {                    /* n % 3 == 1 or 2 */
        idx_t k  = n / 3;
        seq_t ak = a(k);
        result = -ak;
    }

    /* Store result in cache ----------------------------------------------- */
    cache[(size_t)n] = result;
    computed[(size_t)n] = 1;
    return result;
}
