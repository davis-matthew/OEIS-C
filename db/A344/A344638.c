/*
 * A344638  Number of compositions of graph K_4 X P_n.
 * https://oeis.org/A344638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 112*a(n-1) - 346*a(n-2) + 306*a(n-3) - 57*a(n-4) +
 *            2*a(n-5) (for n >= 6), with initial values a(1)=15, a(2)=1548,
 *            a(3)=168386, a(4)=18328142, a(5)=1994963186
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* array storing computed terms, index 0 is unused */
static size_t cap   = 0;         /* number of free slots in cache (>= last index + 1) */
static idx_t last_computed = 0;  /* largest n for which cache[n] is valid */

/* Ensure that cache has room for indices 0 .. needed-1 */
static void grow_cache(size_t needed)
{
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (tmp == NULL) {
            abort();                     /* out‑of‑memory: terminate program */
        }
        cache = tmp;
        cap   = needed;
    }
}

seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 5 */
    static const seq_t init[5] = {
        (seq_t)15ULL,
        (seq_t)1548ULL,
        (seq_t)168386ULL,
        (seq_t)18328142ULL,
        (seq_t)1994963186ULL
    };

    if (n <= 5) {
        return init[n - 1];
    }

    /* Cache must hold indices up to n (including 0). */
    grow_cache((size_t)(n + 1));

    /* Store first five values if not yet stored. */
    if (last_computed < 5) {
        for (size_t i = 1; i <= 5; ++i) {
            cache[i] = init[i - 1];
        }
        last_computed = 5;
    }

    /* Compute missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term = (seq_t)112 * cache[i - 1];
        term -= (seq_t)346 * cache[i - 2];
        term += (seq_t)306 * cache[i - 3];
        term -= (seq_t)57  * cache[i - 4];
        term += (seq_t)2   * cache[i - 5];
        cache[i] = term;
    }

    last_computed = n;
    return cache[n];
}
