/*
 * A261029  Number of ways to write n in the form F(x,y,z) = x^3 + y^3 + z^3 -
 *          3xyz, where 0 <= x <= y <= z and z >= x+1.
 * https://oeis.org/A261029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261029
   a(n) = number of ways to write n as
          F(x,y,z) = x³ + y³ + z³ – 3xyz
          with 0 ≤ x ≤ y ≤ z and z ≥ x + 1 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *count_cache = NULL;  /* array of a(0)..a(max_computed) */
static idx_t   cache_size  = 0;    /* number of elements currently stored */

/* Rebuild the cache of counts for all n ≤ limit.  */
static void recompute_cache(idx_t limit)
{
    /* allocate/resize the cache */
    size_t new_size = (size_t)limit + 1;
    count_cache = (seq_t *)realloc(count_cache, new_size * sizeof(seq_t));
    if (!count_cache) exit(EXIT_FAILURE);

    /* initialise all entries to zero */
    for (idx_t i = 0; i < new_size; ++i)
        count_cache[i] = 0;

    cache_size = new_size;

    /* The largest x,y needed are bounded by (limit+2)/3.
       For any valid triple with n ≤ limit we must have x ≤ ( limit - 1 ) / 3,
       hence the same bound suffices for the loops below. */
    idx_t max_xy = (limit + 2) / 3;

    for (idx_t x = 0; x <= max_xy; ++x) {
        int64_t xx = (int64_t)x;
        int64_t xx3 = xx * xx * xx;

        for (idx_t y = x; y <= max_xy; ++y) {
            int64_t yy = (int64_t)y;
            int64_t yy3 = yy * yy * yy;

            /* z must satisfy z ≥ y and z ≥ x + 1 */
            idx_t z_start = (y > x + 1) ? y : (x + 1);

            /* start search from z_start */
            int64_t z = (int64_t)z_start;
            int64_t f_val = xx3 + yy3 + z * z * z - 3 * xx * yy * z;

            while (f_val <= (int64_t)limit) {
                /* increment count for this n */
                seq_t tmp = count_cache[(idx_t)f_val];
                tmp = (seq_t)((uint64_t)tmp + 1);
                count_cache[(idx_t)f_val] = tmp;

                /* prepare next z using the recurrence
                   F(z+1) – F(z) = 3z² + 3z + 1 – 3xy                */
                int64_t inc = 3 * z * z + 3 * z + 1 - 3 * xx * yy;
                ++z;
                f_val += inc;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        /* enlarge the cache to include this n */
        recompute_cache(n);
    }
    return count_cache[n];
}
