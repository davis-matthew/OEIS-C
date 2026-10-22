/*
 * A317646  Expansion of (1 + theta_3(q))^2*(1 + theta_3(q^2))^2/16, where
 *          theta_3() is the Jacobi theta function.
 * https://oeis.org/A317646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to * x^2 + y^2 +
 *            2*z^2 + 2*w^2 = n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root: floor(sqrt(n)) for unsigned 64‑bit integers. */
static inline idx_t uint64_isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1))
        ++r;
    return r;
}

/* Computes a(n).  The function uses a static cache for the
 * number of representations of each value as x^2 + y^2 with
 * x,y≥0.  The cache is enlarged when a larger n is requested.
 */
seq_t a(idx_t n)
{
    static seq_t *rep2 = NULL;      /* array of counts for x^2 + y^2 = m */
    static idx_t rep2_size = 0;     /* current allocation size */

    /* Rebuild cache if necessary. */
    if (n >= rep2_size) {
        if (rep2) {
            free(rep2);
        }
        rep2_size = n + 1;
        rep2 = (seq_t *)malloc((size_t)rep2_size * sizeof(seq_t));
        if (!rep2)
            return (seq_t)0;       /* out of memory – return 0 as a safe value */
        /* initialise */
        for (idx_t i = 0; i < rep2_size; ++i)
            rep2[i] = (seq_t)0;

        /* precompute ordered pairs (x,y) with x^2 + y^2 ≤ n */
        idx_t lim_xy = uint64_isqrt(n);
        for (idx_t x = 0; x <= lim_xy; ++x) {
            idx_t x2 = x * x;
            for (idx_t y = 0; y <= lim_xy; ++y) {
                idx_t val = x2 + y * y;
                if (val > n)
                    break;
                rep2[val] += (seq_t)1;
            }
        }
    }

    /* Count solutions (x,y,z,w).  The loops run over all
     * z,w such that 2*z^2 ≤ n and 2*w^2 ≤ n - 2*z^2.
     */
    seq_t total = (seq_t)0;

    idx_t lim_z = uint64_isqrt(n / 2);
    for (idx_t z = 0; z <= lim_z; ++z) {
        idx_t z_term = 2 * z * z;
        if (z_term > n)
            break;

        idx_t rem_after_z = n - z_term;
        idx_t lim_w = uint64_isqrt(rem_after_z / 2);
        for (idx_t w = 0; w <= lim_w; ++w) {
            idx_t w_term = 2 * w * w;
            idx_t rem = rem_after_z - w_term;
            if (rem <= n)
                total += rep2[rem];
        }
    }

    return total;
}
