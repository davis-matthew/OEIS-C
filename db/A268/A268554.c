/*
 * A268554  Diagonal of the rational function 1/((1 - w - u v) * (1 - x y - x
 *          z - y z)).
 * https://oeis.org/A268554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)! * (3*n)! / ((n!)^3 * (2*n)!^2) /* Recurrence: a(0)
 *            = 1 * a(n) = a(n-1) * 6*(4*n-1)*(3*n-1)*(3*n-2)*(4*n-3) /
 *            (n^3*(2*n-1)) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/gcd.h"          /* provides gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 0 <= i < cache_size */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialize cache for a(0) */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* out of memory: give 0 as a safe fallback */
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        cache_size = 1;
    }

    /* if n < cache_size, result is already in cache */
    if (n < cache_size)
        return cache[n];

    /* ensure cache has space for indices up to n */
    size_t old_size = cache_size;
    if (n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* out of memory */
        memcpy(new_cache, cache, old_size * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute a(i) for all i in [old_size .. n] using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        /* factors for the recurrence
           numerator factors: 6, 4*i-1, 3*i-1, 3*i-2, 4*i-3
           denominator factors: i, i, i, 2*i-1
        */
        seq_t value = cache[i - 1];
        seq_t nums[5];
        seq_t dens[4];

        nums[0] = (seq_t)6;
        nums[1] = (seq_t)(4 * i - 1);
        nums[2] = (seq_t)(3 * i - 1);
        nums[3] = (seq_t)(3 * i - 2);
        nums[4] = (seq_t)(4 * i - 3);

        dens[0] = (seq_t)i;
        dens[1] = (seq_t)i;
        dens[2] = (seq_t)i;
        dens[3] = (seq_t)(2 * i - 1);

        /* reduce numerator factors by any cancelation with denominator factors */
        for (int ni = 0; ni < 5; ++ni) {
            for (int di = 0; di < 4; ++di) {
                if (dens[di] == (seq_t)1) continue;
                seq_t g = gcd(nums[ni], dens[di]);
                if (g == (seq_t)1) continue;
                nums[ni] = nums[ni] / g;
                dens[di] = dens[di] / g;
            }
        }

        /* multiply remaining numerator factors into value, reducing with
           remaining denominator factors after each multiplication to keep numbers small */
        for (int ni = 0; ni < 5; ++ni) {
            if (nums[ni] == (seq_t)1) continue;
            value = value * nums[ni];
            for (int di = 0; di < 4; ++di) {
                if (dens[di] == (seq_t)1) continue;
                seq_t g = gcd(value, dens[di]);
                if (g == (seq_t)1) continue;
                value = value / g;
                dens[di] = dens[di] / g;
            }
        }

        /* any remaining denominators must divide the result exactly */
        for (int di = 0; di < 4; ++di) {
            if (dens[di] == (seq_t)1) continue;
            value = value / dens[di];
        }

        cache[i] = value;
    }

    return cache[n];
}
