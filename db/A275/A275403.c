/*
 * A275403  Number of 4_X_n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 11*a(n-2) + 25*a(n-3) + 2*a(n-4) - 24*a(n-5)
 *            for n > 6 Initial terms (offset 1): a(1) = 6, a(2) = 31, a(3) =
 *            178, a(4) = 1011, a(5) = 5758, a(6) = 32771
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for offset 1, but guard against accidental use */
        return (seq_t)0;
    }

    /* static cache that remembers previously computed values */
    static seq_t   *cache   = NULL;
    static size_t   cap     = 0;   /* allocated capacity (number of elements)   */
    static idx_t    size    = 0;   /* highest n that has been computed          */

    /* initialise the first six terms if not yet done */
    if (size == 0) {
        const size_t init_cap = 7;   /* indices 0..6 (index 0 unused) */
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (!cache) {
            /* allocation failed – signal failure with zero (should never happen) */
            return (seq_t)0;
        }
        cap   = init_cap;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)31;
        cache[3] = (seq_t)178;
        cache[4] = (seq_t)1011;
        cache[5] = (seq_t)5758;
        cache[6] = (seq_t)32771;
        size   = 6;
    }

    /* if the requested value is already cached, return it immediately */
    if (n <= size) {
        return cache[n];
    }

    /* grow the cache if necessary to hold the new index */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < (size_t)n + 1) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            return (seq_t)0;   /* allocation failed */
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute the missing terms up to n using the recurrence */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = ((seq_t)3 * a1) + ((seq_t)11 * a2) + ((seq_t)25 * a3)
                 + ((seq_t)2  * a4) + ((seq_t)-24 * a5);  /* the negative is handled by unsigned wrap‑around */
    }

    size = n;
    return cache[n];
}
