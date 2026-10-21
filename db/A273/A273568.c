/*
 * A273568  Number of ordered ways to write n as w^2 + x^2 + y^2 + z^2 with w
 *          + x + 2*y - 4*z twice a nonnegative cube, where w is an integer
 *          and x,y,z are nonnegative integers.
 * https://oeis.org/A273568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered tuples (w,x,y,z) with * w integer,
 *            x,y,z non‑negative, * w^2 + x^2 + y^2 + z^2 = n, * and w + x +
 *            2*y - 4*z = 2*c^3 for some non‑negative integer c
 * Range:     u8 n <= 5147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(n)) */
static inline idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (high > low) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid) high = mid - 1;
        else low = mid;
    }
    return low;
}

/* integer cube root: floor(cuberoot(n)) */
static inline idx_t icbrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (high > low) {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Count representations up to N inclusive, store in cnt[0…N] */
static void compute_counts(idx_t N, seq_t *cnt)
{
    /* zero the table */
    for (idx_t i = 0; i <= N; ++i)
        cnt[i] = 0;

    idx_t S = isqrt(N);
    /* we treat S as small enough to cast to int safely for typical test bounds */
    int maxS = (int)S;

    for (int w = -maxS; w <= maxS; ++w) {
        idx_t w2 = (idx_t)w * (idx_t)w;
        for (int x = 0; x <= maxS; ++x) {
            idx_t sum_wx = w2 + (idx_t)x * (idx_t)x;
            for (int y = 0; y <= maxS; ++y) {
                idx_t sum_wxy = sum_wx + (idx_t)y * (idx_t)y;
                for (int z = 0; z <= maxS; ++z) {
                    idx_t sum = sum_wxy + (idx_t)z * (idx_t)z;
                    if (sum > N) break;                /* further z only increases sum */

                    /* compute rem = w + x + 2*y - 4*z */
                    int rem = w + x + 2 * y - 4 * z;
                    if (rem < 0 || (rem & 1)) continue;  /* need rem even and non‑negative */
                    idx_t k = (idx_t)(rem >> 1);        /* k = rem / 2 */
                    idx_t c = icbrt(k);
                    if ( (__int128)c * c * c == k )
                        cnt[sum] += 1;
                }
            }
        }
    }
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        idx_t new_limit = n + 1;
        if (cache) free(cache);
        cache = (seq_t *)malloc(new_limit * sizeof(seq_t));
        compute_counts(n, cache);
        cache_size = new_limit;
    }
    return cache[n];
}
