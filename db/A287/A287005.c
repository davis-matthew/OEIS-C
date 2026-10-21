/*
 * A287005  Number of connected dominating sets on the n-Moebius ladder.
 * https://oeis.org/A287005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A287005: Number of connected dominating sets on the n‑Moebius ladder
    Formula: a(n) = 6*a(n-1) - 11*a(n-2) + 4*a(n-3) + 5*a(n-4) - 2*a(n-5) - a(n-6)
    Initial values: a(1)=9, a(2)=13, a(3)=49, a(4)=129, a(5)=361, a(6)=989
*/

#include <stddef.h>
#include <stdlib.h>

/* Initial six terms ----------------------------------------------------- */
static const seq_t init[6] = {
    (seq_t)9,
    (seq_t)13,
    (seq_t)49,
    (seq_t)129,
    (seq_t)361,
    (seq_t)989
};

/* Dynamic cache for computed terms ------------------------------------- */
static seq_t *cache = NULL;      /* array holding a(1..n-1) */
static size_t cache_cap = 0;     /* allocated size of cache */ 

/*-----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, return 0 for safety */
    if (n <= 6) return init[(size_t)n - 1]; /* return one of the known values */

    /* Ensure the cache is large enough to hold n terms */
    size_t needed = (size_t)n;
    if (cache == NULL || cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure – return 0 */ }
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialize the first six terms */
    for (size_t i = 0; i < 6; ++i) cache[i] = init[i];

    /* Compute the remaining terms up to a(n) */
    for (size_t i = 6; i < (size_t)n; ++i) {
        seq_t term = 6 * cache[i - 1] + 4 * cache[i - 3] + 5 * cache[i - 4];
        term -= 11 * cache[i - 2];
        term -= 2 * cache[i - 5];
        term -= cache[i - 6];
        cache[i] = term;
    }

    return cache[(size_t)n - 1];
}
