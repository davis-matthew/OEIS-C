/*
 * A386287  Values of w in the quartets (3, u, v, w) of type 2; i.e., values
 *          of v for solutions to 3(3 + u) = v(v - w), in distinct positive
 *          integers, with v > 1, sorted by nondecreasing values of u; see
 *          A386285.
 * https://oeis.org/A386287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386287: compute w values from the equation
   3(3 + u) = v(v – w) by enumerating u ≥ 1 (u ≠ 3),
   v | 3(3+u), v > 1, v ≠ 3, v ≠ u;
   then w = v – (3(3+u))/v, with w > 0 and w ≠ 3, w ≠ u, w ≠ v.
   The w’s are emitted in order of increasing u, and for a fixed u
   in order of increasing v.  The function a(n) returns the nth such w. */

#include <stdlib.h>
#include <stdint.h>

/* comparison function for qsort on idx_t */
static int cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* cache for the computed sequence values */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t  next_u    = 1;   /* next u to process */

/* add a new value to the cache, expanding if needed */
static void cache_add(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* process a single u, appending all valid w values to the cache */
static void process_u(idx_t u)
{
    if (u == 3)   /* 3 would duplicate the 3 in the quartet */
        return;

    idx_t L = 3 * (3 + u);

    /* collect divisors of L other than 1, excluding u and 3 */
    static idx_t vbuf[2048];   /* large enough for any L in the target range */
    size_t vcnt = 0;

    for (idx_t d = 2; d * d <= L; ++d) {
        if (L % d == 0) {
            idx_t v1 = d;
            if (v1 != 3 && v1 != u)
                vbuf[vcnt++] = v1;
            idx_t v2 = L / d;
            if (v2 != v1 && v2 != 3 && v2 != u)
                vbuf[vcnt++] = v2;
        }
    }

    /* include L itself as a divisor (unless excluded) */
    if (L >= 2 && L != 3 && L != u)
        vbuf[vcnt++] = L;

    if (vcnt == 0)
        return;

    qsort(vbuf, vcnt, sizeof(idx_t), cmp_idx);

    for (size_t i = 0; i < vcnt; ++i) {
        idx_t v = vbuf[i];
        /* compute w = v - L/v, using signed arithmetic to detect negatives */
        int64_t diff = (int64_t)v - (int64_t)(L / v);
        if (diff <= 0)
            continue; /* w must be positive */
        seq_t w = (seq_t)diff;
        if (w == 3 || w == u || w == v)
            continue; /* all four numbers must be distinct */
        cache_add(w);
    }
}

seq_t a(idx_t n)
{
    /* required index starts at 1 */
    while (cache_len < n) {
        process_u(next_u);
        ++next_u;
    }
    return cache[n - 1];
}
