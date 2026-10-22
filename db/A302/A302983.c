/*
 * A302983  Number of ways to write n as x^2 + 2*y^2 + 2^z + 3*2^w with
 *          x,y,z,w nonnegative integers.
 * https://oeis.org/A302983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to n = x^2 +
 *            2*y^2 + 2^z + 3*2^w (x, y, z, w ≥ 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Allocate or grow the auxiliary array f that stores
       the number of representations of m as x^2 + 2*y^2. */
    static seq_t *f = NULL;
    static idx_t f_cap = 0;

    if (f_cap < n + 1) {
        free(f);
        f = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        f_cap = n + 1;
    }

    /* Zero the array up to n. */
    memset(f, 0, (n + 1) * sizeof(seq_t));

    /* Precompute all values m = x^2 + 2*y^2 ≤ n. */
    idx_t max_x = 0;
    while ((max_x + 1) * (max_x + 1) <= n) max_x++;
    idx_t max_y = 0;
    while (2 * (max_y + 1) * (max_y + 1) <= n) max_y++;

    for (idx_t x = 0; x <= max_x; ++x) {
        idx_t x2 = x * x;
        for (idx_t y = 0; y <= max_y; ++y) {
            idx_t val = x2 + 2 * y * y;
            if (val > n) continue;
            f[val] += (seq_t)1;
        }
    }

    /* Count solutions by iterating over the powers of 2. */
    seq_t result = (seq_t)0;

    /* z: 2^z part */
    for (idx_t pow_z = 1; pow_z <= n; /* incremented inside */) {
        idx_t rem_z = n - pow_z;

        /* w: 3*2^w part */
        for (idx_t pow_w = 3; pow_w <= rem_z; /* incremented inside */) {
            idx_t rem = rem_z - pow_w;
            if (rem <= n) {
                result += f[rem];
            }

            /* move to next w (multiply by 2) */
            if (pow_w > rem_z >> 1) break;
            pow_w <<= 1;
        }

        /* move to next z (multiply by 2) */
        if (pow_z > n >> 1) break;
        pow_z <<= 1;
    }

    return result;
}
