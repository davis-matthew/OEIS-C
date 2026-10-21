/*
 * A299267  Partial sums of A299266.
 * https://oeis.org/A299267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (OEIS A299267):
 *   a(0) = 1
 *   a(1) = 6
 *   a(2) = 15
 *   a(3) = 37
 *   a(4) = 74
 *   a(5) = 131
 *   a(6) = 213
 *   a(7) = 330
 *   a(8) = 475
 *   a(9) = 653
 *   a(10) = 882
 *   a(11) = 1163
 *   a(12) = 1485
 *   For n > 12:
 *     a(n) = 2*a(n-1) - 2*a(n-2) + 3*a(n-3) - 2*a(n-4)
 *            - 2*a(n-7) + 3*a(n-8) - 2*a(n-9) + 2*a(n-10) - a(n-11)
 */

#include <stddef.h>   /* for size_t */

/* A static cache large enough for all requested indices.  20001
 * comfortably covers the required n ≤ 10 000 and gives a small safety margin.
 */
#define MAX_CACHE_SIZE 20001

static seq_t cache[MAX_CACHE_SIZE];
static idx_t  computed_up_to = 0;        /* number of values computed (size of valid data);

/* Pre‑initialised values for n = 0 … 12. */
static const seq_t init_vals[13] = {
    1, 6, 15, 37, 74, 131, 213, 330, 475, 653, 882, 1163, 1485
};

/* Store the first 13 terms in the cache on first use. */
static void init_cache(void)
{
    for (idx_t i = 0; i <= 12; ++i)
        cache[i] = init_vals[i];
    computed_up_to = 13;               /* indices 0 … 12 are now valid */
}

/* Computes a(n) for any n >= 0.  The returned value is held in cache[n] for future calls. */
seq_t a(idx_t n)
{
    /* Initialise cache on the first call. */
    if (!computed_up_to)
        init_cache();

    /* If the requested index is already computed, simply return it. */
    if (n < computed_up_to)
        return cache[n];

    /* Compute all missing entries up to and including n. */
    for (idx_t i = computed_up_to; i <= n; ++i)
    {
        /* The recurrence is guaranteed valid for i > 12, and for i <= 12
         * the initial values are already present.  All accesses below are
         * therefore within bounds for the already computed entries. */
        seq_t val = 2 * cache[i - 1]
                    - 2 * cache[i - 2]
                    + 3 * cache[i - 3]
                    - 2 * cache[i - 4]
                    - 2 * cache[i - 7]
                    + 3 * cache[i - 8]
                    - 2 * cache[i - 9]
                    + 2 * cache[i -10]
                    - cache[i -11];
        cache[i] = val;
    }

    /* Update how far we have computed. */
    computed_up_to = n + 1;
    return cache[n];
}
