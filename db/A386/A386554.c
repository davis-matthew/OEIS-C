/*
 * A386554  Number of 2-colorings of an 4 X 4 X 4 grid, up to rotational
 *          symmetry, by the number of black cells.
 * https://oeis.org/A386554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of z^n in 1/24( * (1+z)^64 * + 8 (1+z)^4
 *            (1+z^3)^20 * + 9 (1+z^2)^32 * + 6 (1+z^4)^16 * )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static void init_cache(void);
static seq_t cache[65];
static bool ready = false;

seq_t a(idx_t n)
{
    if (!ready) init_cache();
    if (n > 64) return 0;
    return cache[n];
}

static void init_cache(void)
{
    /* Pre‑compute binomial coefficients up to 64 */
    static seq_t binom[65][65];
    for (int i = 0; i <= 64; ++i) {
        binom[i][0] = 1;
        binom[i][i] = 1;
        for (int j = 1; j < i; ++j)
            binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
    }

    /* Auxiliary arrays */
    seq_t B1[65] = {0}, B2[65] = {0}, B3[65] = {0}, B4[65] = {0};

    /* B1: (1+z)^64 */
    for (int n = 0; n <= 64; ++n)
        B1[n] = binom[64][n];

    /* B2: (1+z)^4 (1+z^3)^20 */
    for (int i = 0; i <= 4; ++i) {
        seq_t c1 = binom[4][i];
        for (int j = 0; j <= 20; ++j) {
            int n = i + 3 * j;
            if (n > 64) continue;
            B2[n] += c1 * binom[20][j];
        }
    }

    /* B3: (1+z^2)^32 */
    for (int k = 0; k <= 32; ++k) {
        int n = 2 * k;
        if (n > 64) continue;
        B3[n] += binom[32][k];
    }

    /* B4: (1+z^4)^16 */
    for (int k = 0; k <= 16; ++k) {
        int n = 4 * k;
        if (n > 64) continue;
        B4[n] += binom[16][k];
    }

    /* Compute final values */
    for (int n = 0; n <= 64; ++n) {
        seq_t sum = B1[n];
        sum += (8 * B2[n]);
        sum += (9 * B3[n]);
        sum += (6 * B4[n]);
        cache[n] = sum / 24;
    }

    ready = true;
}
