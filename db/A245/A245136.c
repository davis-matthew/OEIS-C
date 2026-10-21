/*
 * A245136  Number of length 6 0..n arrays least squares fitting to a zero
 *          slope straight line, with a single point taken as having zero
 *          slope.
 * https://oeis.org/A245136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 4*a(n-5) * -
 *            5*a(n-6) + 4*a(n-7) - 5*a(n-8) + 8*a(n-9) * - 5*a(n-10) +
 *            4*a(n-11) - 5*a(n-12) + 4*a(n-13) * - 4*a(n-14) + 2*a(n-15) -
 *            a(n-16) + 2*a(n-17) - a(n-18) * for n > 18, with initial values:
 *            * a(1)=8, a(2)=43, a(3)=172, a(4)=505, a(5)=1248,...
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 121, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined indices return 0 */
    if (n == 0) return (seq_t)0;

    /* Seed values for n = 1 … 18.  All cast to seq_t to avoid integer narrowing. */
    static const seq_t init[18] = {
        (seq_t)8,    (seq_t)43,   (seq_t)172,  (seq_t)505,
        (seq_t)1248, (seq_t)2687, (seq_t)5220, (seq_t)9385,
        (seq_t)15868,(seq_t)25539,(seq_t)39428,(seq_t)58805,
        (seq_t)85144,(seq_t)120163,(seq_t)165900,(seq_t)224593,
        (seq_t)298832,(seq_t)391539
    };

    /* Dynamic cache for computed terms.  Static ensures memoisation across calls. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* allocated capacity in terms */

    /* Initialise the cache on first entry with the 18 seed terms. */
    if (cache_len == 0) {
        cache_cap = 18;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 18; ++i) cache[i] = init[i];
        cache_len = 18;
    }

    /* If the requested index is already computed, return it immediately. */
    if ((idx_t)cache_len >= n) return cache[(size_t)(n - 1)];

    /* Compute terms up to n using the recurrence. */
    while (cache_len < (size_t)n) {
        /* Ensure there is room for the next term. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        size_t i = cache_len;   /* i corresponds to term a(i+1) */
        seq_t term = (seq_t)0;

        /* Positive contributions.  All multiplications use seq_t constants
           to avoid accidental integer promotion loss. */
        term += (seq_t)2 * cache[i - 1];
        term += (seq_t)2 * cache[i - 3];
        term += (seq_t)4 * cache[i - 5];
        term += (seq_t)4 * cache[i - 7];
        term += (seq_t)8 * cache[i - 9];
        term += (seq_t)4 * cache[i - 11];
        term += (seq_t)4 * cache[i - 13];
        term += (seq_t)2 * cache[i - 15];
        term += (seq_t)2 * cache[i - 17];

        /* Negative contributions. */
        term -= cache[i - 2];
        term -= (seq_t)4 * cache[i - 4];
        term -= (seq_t)5 * cache[i - 6];
        term -= (seq_t)5 * cache[i - 8];
        term -= (seq_t)5 * cache[i - 10];
        term -= (seq_t)5 * cache[i - 12];
        term -= (seq_t)4 * cache[i - 14];
        term -= cache[i - 16];
        term -= cache[i - 18];

        cache[i] = term;
        ++cache_len;
    }

    return cache[(size_t)(n - 1)];
}
