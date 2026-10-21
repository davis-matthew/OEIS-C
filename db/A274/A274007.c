/*
 * A274007  Number of ordered ways to write n as x^5 + 2*y^5 + z*(3*z-1)/2 +
 *          w*(3*w+1)/2, where x,y,z,w are nonnegative integers.
 * https://oeis.org/A274007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,z,w) non‑negative integers | x^5 + 2*y^5 +
 *            z*(3*z-1)/2 + w*(3*w+1)/2 = n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* small helper: compute k^5 (k fits in idx_t for our limits) */
static inline idx_t pow5(idx_t k)
{
    idx_t v = 1;
    for (idx_t i = 0; i < 5; ++i)
        v *= k;
    return v;
}

seq_t a(idx_t n)
{
    /* determine maximum values for each variable */
    idx_t max_x = 0;
    while (pow5(max_x + 1) <= n)
        ++max_x;

    idx_t max_y = 0;
    while (pow5(max_y + 1) <= (n >> 1))   /* 2*y^5 <= n  */
        ++max_y;

    idx_t max_z = 0;
    while ((max_z + 1) * (3 * (max_z + 1) - 1) / 2 <= n)
        ++max_z;

    idx_t max_w = 0;
    while ((max_w + 1) * (3 * (max_w + 1) + 1) / 2 <= n)
        ++max_w;

    /* allocate small tables */
    idx_t *pow5x = (idx_t *)malloc((size_t)(max_x + 1) * sizeof(idx_t));
    idx_t *pow5y = (idx_t *)malloc((size_t)(max_y + 1) * sizeof(idx_t));
    idx_t *zterm = (idx_t *)malloc((size_t)(max_z + 1) * sizeof(idx_t));
    idx_t *wterm = (idx_t *)malloc((size_t)(max_w + 1) * sizeof(idx_t));

    if (!pow5x || !pow5y || !zterm || !wterm) {
        free(pow5x); free(pow5y); free(zterm); free(wterm);
        return (seq_t)0;   /* out of memory – safe fallback */
    }

    /* fill tables */
    for (idx_t i = 0; i <= max_x; ++i)
        pow5x[i] = pow5(i);

    for (idx_t i = 0; i <= max_y; ++i)
        pow5y[i] = pow5(i);

    for (idx_t i = 0; i <= max_z; ++i)
        zterm[i] = i * (3 * i - 1) / 2;

    for (idx_t i = 0; i <= max_w; ++i)
        wterm[i] = i * (3 * i + 1) / 2;

    /* count solutions */
    idx_t cnt = 0;

    for (idx_t x = 0; x <= max_x; ++x) {
        idx_t valx = pow5x[x];
        if (valx > n)
            break;
        for (idx_t y = 0; y <= max_y; ++y) {
            idx_t valxy = valx + 2 * pow5y[y];
            if (valxy > n)
                break;
            for (idx_t z = 0; z <= max_z; ++z) {
                idx_t valxyz = valxy + zterm[z];
                if (valxyz > n)
                    break;
                for (idx_t w = 0; w <= max_w; ++w) {
                    idx_t sum = valxyz + wterm[w];
                    if (sum == n)
                        ++cnt;
                    else if (sum > n)
                        break;
                }
            }
        }
    }

    free(pow5x);
    free(pow5y);
    free(zterm);
    free(wterm);

    return (seq_t)cnt;
}
