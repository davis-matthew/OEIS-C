/*
 * A278444  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 43", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 10, a(2) = 100, a(3) = 1011,
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
// a(4) = 10100, a(5) = 101011, a(6) = 1010000.
// For n >= 7 the numbers satisfy the linear recurrence:
//     a(n) = 10*a(n-1) + a(n-2) - 10*a(n-3) + 100*a(n-4)
//           - 1000*a(n-5) - 100*a(n-6) + 1000*a(n-7).
// This recurrence comes from the rational generating function given in the OEIS entry.
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache array for computed terms   */
static idx_t  cache_size = 0;        /* number of entries currently stored */

/* Ensure that terms up to index n are computed and cached. */
static void ensure_cache(idx_t n)
{
    /* Grow the cache if necessary.  Keep it at least twice as big as requested
       to reduce reallocations.  Start with a minimal size of 8 so that the 7
       initial terms are stored. */
    if (cache_size <= n) {
        idx_t new_size = (cache_size * 2 > n + 1) ? cache_size * 2 : n + 1;
        if (new_size < 8) new_size = 8;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;

        /* Initialise the first seven terms if they are not already set.  The
           base values are the decimal representations of the binary strings
           listed in the OEIS entry. */
        const size_t base_len = 7;
        static const seq_t base[7] = {
            (seq_t)1,           /* a(0) */
            (seq_t)10,          /* a(1) */
            (seq_t)100,         /* a(2) */
            (seq_t)1011,        /* a(3) */
            (seq_t)10100,       /* a(4) */
            (seq_t)101011,      /* a(5) */
            (seq_t)1010000      /* a(6) */
        };

        for (idx_t i = cache_size; i < base_len && i < new_size; ++i)
            cache[i] = base[i];

        /* Compute new terms using the recurrence. */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 7) continue;   /* already initialised */

            seq_t value = 10 * cache[i - 1] + cache[i - 2];
            value -= 10 * cache[i - 3];
            value += 100 * cache[i - 4];
            value -= 1000 * cache[i - 5];
            value -= 100 * cache[i - 6];
            value += 1000 * cache[i - 7];

            cache[i] = value;
        }

        cache_size = new_size;
    }
}

/* Return the n-th term of A278444 (decimal representation of the binary
   string of the x‑axis). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
