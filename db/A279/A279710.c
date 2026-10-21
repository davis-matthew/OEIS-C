/*
 * A279710  Number of 3 X n 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A279710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A279710
   Empirical recurrence:
     a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 4*a(n-4)
            - a(n-5) - a(n-7) - a(n-8)
   for n > 8.
   Initial values for 1 ≤ n ≤ 8:
     a(1)=4, a(2)=5, a(3)=11, a(4)=22, a(5)=47,
     a(6)=102, a(7)=224, a(8)=494.
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] unused, indices 1.. */
static idx_t cache_cap = 0;          /* allocated capacity (max index) */
static idx_t cache_size = 0;         /* highest index computed */

/* Expand cache to at least n + 1 elements (index n is valid).  */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) {
        /* malloc failure – abort as per contest rules */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n < 1 */

    /* initialise cache on first call */
    if (!cache) {
        cache = (seq_t *)calloc(1, sizeof(seq_t)); /* at least index 0 */
        if (!cache) abort();
        cache_cap = 0;
        cache_size = 0;
    }

    ensure_cache(n);

    /* compute missing values up to n */
    if (cache_size < n) {
        /* base values for 1 .. 8 */
        static const seq_t base[9] = {
            0,            /* unused */
            4, 5, 11, 22, 47,
            102, 224, (seq_t)494
        };
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 8)
                cache[i] = base[i];
            else {
                /* recurrence – all operations are on unsigned types, so
                   no overflow occurs while the result is non‑negative. */
                cache[i] = (seq_t)3 * cache[i-1]
                           - (seq_t)2 * cache[i-2]
                           - cache[i-3]
                           + (seq_t)4 * cache[i-4]
                           - cache[i-5]
                           - cache[i-7]
                           - cache[i-8];
            }
        }
        cache_size = n;
    }
    return cache[n];
}
