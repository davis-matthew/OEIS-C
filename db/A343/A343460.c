/*
 * A343460  Number of ways to write n as x^6 + y^3 + z*(3*z+1)/2 + 2^k, where
 *          x and y are nonnegative integers, z is an integer and k is a
 *          positive integer.
 * https://oeis.org/A343460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (x,y,z,k) with x>=0, y>=0, z∈ℤ, k>=1 such that
 *            n = x^6 + y^3 + z(3*z+1)/2 + 2^k (OEIS A343460)
 * Range:     u8 n <= 1313, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static inline bool is_power_of_two(seq_t v)
{
    /* return true if v is a power of two and v >= 2 */
    if (v < (seq_t)2) return false;
    while (v % (seq_t)2 == 0) v /= (seq_t)2;
    return v == (seq_t)1;
}

seq_t a(idx_t n)
{
    /* small n cannot be represented because the smallest summand is 2^1 = 2 */
    if (n < 2) return 0;

    /* cache n as seq_t for arithmetic */
    seq_t n_val = (seq_t)n;
    seq_t max_base = n_val - 2;   /* base = x^6 + y^3 + z(3*z+1)/2 must be <= max_base */

    /* determine maximum x with x^6 <= max_base */
    idx_t maxX = 0;
    for (idx_t x = 0; ; ++x) {
        seq_t p = 1;
        for (int i = 0; i < 6; ++i) p *= (seq_t)x;
        if (p > max_base) break;
        maxX = x;
    }

    /* determine maximum y with y^3 <= max_base */
    idx_t maxY = 0;
    for (idx_t y = 0; ; ++y) {
        seq_t p = 1;
        for (int i = 0; i < 3; ++i) p *= (seq_t)y;
        if (p > max_base) break;
        maxY = y;
    }

    /* determine maximum |z| whose quadratic term is <= max_base
       (both positive and negative z will be considered) */
    idx_t maxZ = 0;
    for (idx_t z = 0; ; ++z) {
        seq_t pos_val = (seq_t)z * ((seq_t)3 * z + 1) / 2;   /* z >= 0 */
        seq_t neg_val = (seq_t)z * ((seq_t)3 * z - 1) / 2;   /* z <= 0 (value for -z) */
        if (pos_val > max_base && neg_val > max_base) break;
        maxZ = z;
    }

    seq_t count = 0;

    for (idx_t x = 0; x <= maxX; ++x) {
        /* x^6 */
        seq_t x6 = 1;
        for (int i = 0; i < 6; ++i) x6 *= (seq_t)x;

        for (idx_t y = 0; y <= maxY; ++y) {
            /* y^3 */
            seq_t y3 = 1;
            for (int i = 0; i < 3; ++i) y3 *= (seq_t)y;

            seq_t base_xy = x6 + y3;
            if (base_xy > max_base) continue;              /* no room for 2^k */

            /* z = 0 case */
            seq_t rem = n_val - base_xy;
            if (is_power_of_two(rem)) ++count;

            for (idx_t t = 1; t <= maxZ; ++t) {
                /* positive z = +t */
                seq_t t_pos = (seq_t)t * ((seq_t)3 * t + 1) / 2;
                seq_t base = base_xy + t_pos;
                if (base <= max_base) {
                    seq_t diff = n_val - base;
                    if (is_power_of_two(diff)) ++count;
                }

                /* negative z = -t */
                seq_t t_neg = (seq_t)t * ((seq_t)3 * t - 1) / 2;
                base = base_xy + t_neg;
                if (base <= max_base) {
                    seq_t diff = n_val - base;
                    if (is_power_of_two(diff)) ++count;
                }
            }
        }
    }

    return count;
}
