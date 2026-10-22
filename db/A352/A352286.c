/*
 * A352286  Number of ways to write n as w + x^2 + 2*y^2 + 3*z^2 + x*y*z,
 *          where w is 0 or 1, and x,y,z are nonnegative integers.
 * https://oeis.org/A352286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #solutions to n = w + x^2 + 2*y^2 + 3*z^2 + x*y*z with
 *            w∈{0,1} and x,y,z ≥ 0
 * Range:     u8 n <= 1018, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer square‑root (largest r s.t. r*r ≤ n).  Works for any non‑negative n. */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Cached count of triples (x,y,z) that give value v.
   The array is indexed by v and contains the number of triples with
   x^2 + 2*y^2 + 3*z^2 + x*y*z = v.  */
static seq_t *cnt_cache = NULL;   /* array of counts, indexed by value v   */
static idx_t  cache_limit = 0;    /* highest value for which cnt_cache is valid */

/* Build or rebuild the cache up to and including 'max'.                     */
static void build_cache(idx_t max)
{
    /* Allocate a fresh array of size max+1, zeroed.                        */
    seq_t *new_cache = (seq_t *)calloc((size_t)(max + 1), sizeof(seq_t));
    if (!new_cache)          /* allocation failed – leave cache empty           */
        return;

    /* If we already had a cache, free it.                                  */
    if (cnt_cache != NULL)
        free(cnt_cache);

    /* Pre‑compute limits to avoid unnecessary iterations.                  */
    const idx_t lim_x = int_sqrt(max);
    const idx_t lim_y = int_sqrt(max / 2);   /* because 2*y^2 ≤ max */
    const idx_t lim_z = int_sqrt(max / 3);   /* because 3*z^2 ≤ max */

    for (idx_t x = 0; x <= lim_x; ++x) {
        const seq_t sx2 = (seq_t)x * (seq_t)x;
        for (idx_t y = 0; y <= lim_y; ++y) {
            const seq_t sy2 = (seq_t)y * (seq_t)y;
            const seq_t base = sx2 + (seq_t)2 * sy2;
            if (base > (seq_t)max) break;          /* base already too large */

            for (idx_t z = 0; z <= lim_z; ++z) {
                const seq_t sz2 = (seq_t)z * (seq_t)z;
                const seq_t term = (seq_t)3 * sz2 + (seq_t)x * (seq_t)y * (seq_t)z;
                const seq_t v = base + term;
                if (v > (seq_t)max) break;          /* further z will only increase v */
                ++new_cache[(size_t)v];              /* count this triple */
            }
        }
    }

    /* Save the new cache.                                                    */
    cnt_cache    = new_cache;
    cache_limit  = max;
}

/* Public function -----------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure the cache contains all counts up to n.
       Build an initial cache for 0 if nothing has been constructed yet.     */
    if (cnt_cache == NULL)
        build_cache(0);

    if (n > cache_limit)
        build_cache(n);

    /* The number of representations of n is the number of triples that
       give n (w = 0) plus the number that give n-1 (w = 1).                */
    seq_t res = 0;
    if (cnt_cache != NULL) {
        res += cnt_cache[(size_t)n];
        if (n > 0)
            res += cnt_cache[(size_t)(n - 1)];
    }
    return res;
}
