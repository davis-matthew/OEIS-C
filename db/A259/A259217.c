/*
 * A259217  Number of (n+1) X (3+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0011 or 0101.
 * https://oeis.org/A259217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - a(n-3) + 2*a(n-4); Initial values:
 *            a(1)=24, a(2)=40, a(3)=66, a(4)=112
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE 20000

/* static cache for the first MAX_CACHE terms */
static seq_t cache[MAX_CACHE + 1] = {0};

/* highest index already computed */
static idx_t computed_max = 0;

/* first time we encounter n we set base terms */
static int initialized = 0;

seq_t a(idx_t n)
{
    /* initialise base terms on first call */
    if (!initialized) {
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)40;
        cache[3] = (seq_t)66;
        cache[4] = (seq_t)112;
        computed_max = 4;
        initialized = 1;
    }

    /* if requested n already cached, return it */
    if (n <= computed_max) {
        return cache[n];
    }

    /* compute forward until we reach n, storing values up to MAX_CACHE */
    while (computed_max < n) {
        /* last four terms that have been computed so far */
        seq_t a_nm4 = cache[computed_max - 3];
        seq_t a_nm3 = cache[computed_max - 2];
        seq_t a_nm2 = cache[computed_max - 1];
        seq_t a_nm1 = cache[computed_max];

        /* recurrence: a(k+1) = 4*a(k) - 4*a(k-1) - a(k-2) + 2*a(k-3) */
        seq_t val = (seq_t)(4 * a_nm1
                            - 4 * a_nm2
                            - a_nm3
                            + 2 * a_nm4);

        /* advance to next index */
        computed_max++;

        /* store the value if within the cache limit */
        if (computed_max <= MAX_CACHE) {
            cache[computed_max] = val;
        }
    }

    /* after the loop we have computed n and stored it if within cache */
    return cache[n];
}
