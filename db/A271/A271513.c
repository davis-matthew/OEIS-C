/*
 * A271513  Number of ordered ways to write n as w^2 + x^2 + y^2 + z^2 with
 *          3*x^2 + 4*y^2 + 9*z^2 a square, where w, x, y and z are
 *          nonnegative integers.
 * https://oeis.org/A271513
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

/* OEIS A271513
   a(n) = number of ordered quadruples (w,x,y,z) of non‑negative integers
   with w^2+x^2+y^2+z^2 = n  and 3·x^2+4·y^2+9·z^2 a perfect square.
   The function recomputes values up to the required index caching the
   results for future calls. */

#include <stdlib.h>
#include <stdint.h>

/* integer square‑root: floor(sqrt(n)) for unsigned 64‑bit values */
static idx_t floor_isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* static cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* recompute the sequence values up to and including N */
static void recompute(idx_t N)
{
    /* allocate new cache array */
    idx_t new_cap = N + 1;
    seq_t *new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    for (idx_t i = 0; i < new_cap; ++i) new_cache[i] = 0;

    /* pre‑compute w^2 table up to sqrt(N) */
    idx_t w_limit = floor_isqrt(N);
    idx_t *w_sq = (idx_t *)malloc((size_t)(w_limit + 1) * sizeof(idx_t));
    if (!w_sq) { free(new_cache); exit(EXIT_FAILURE); }
    for (idx_t w = 0; w <= w_limit; ++w) w_sq[w] = w * w;

    /* main search loops over x, y, z */
    idx_t x_limit = floor_isqrt(N);
    for (idx_t x = 0; x <= x_limit; ++x) {
        idx_t x_sq = x * x;
        idx_t remaining_x = N - x_sq;
        idx_t y_limit = floor_isqrt(remaining_x);
        for (idx_t y = 0; y <= y_limit; ++y) {
            idx_t y_sq = y * y;
            idx_t sum_xy = x_sq + y_sq;
            if (sum_xy > N) continue;
            idx_t remaining_xy = N - sum_xy;
            idx_t z_limit = floor_isqrt(remaining_xy);
            for (idx_t z = 0; z <= z_limit; ++z) {
                idx_t z_sq = z * z;
                idx_t sum_xyz = sum_xy + z_sq;
                if (sum_xyz > N) continue;

                /* check the quadratic condition */
                unsigned __int128 t = (unsigned __int128)3 * x_sq
                                    + (unsigned __int128)4 * y_sq
                                    + (unsigned __int128)9 * z_sq;
                idx_t sqrt_t = floor_isqrt((idx_t)t);
                if ((unsigned __int128)sqrt_t * sqrt_t != t) continue;

                /* count all w that fit */
                idx_t remaining_xyz = N - sum_xyz;
                idx_t max_w = floor_isqrt(remaining_xyz);
                for (idx_t w = 0; w <= max_w; ++w) {
                    idx_t idx = sum_xyz + w_sq[w];
                    new_cache[idx] += (seq_t)1;
                }
            }
        }
    }

    free(w_sq);
    if (cache) free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* ensure cache covers at least n */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    recompute(n);
}

/* exported function of the required signature */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
