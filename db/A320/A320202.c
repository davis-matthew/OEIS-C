/*
 * A320202  Number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 14*d^2 = n.
 * https://oeis.org/A320202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of a^2 + 2*b^2 + 4*c^2 +
 *            14*d^2 = n (a,b,c,d∈ℤ). The result is computed directly from the
 *            definition
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for idx_t (unsigned 64‑bit type). */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t r   = 0;
    idx_t bit = (idx_t)1 << (sizeof(idx_t) * 8 - 2); /* highest power of 4 ≤ x */

    while (bit > 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Loop over all possible d values. */
    idx_t d_max = int_sqrt(n / 14ULL);
    for (idx_t d = 0; d <= d_max; ++d) {
        idx_t d2   = d * d;
        idx_t rem1 = n - 14ULL * d2;           /* remaining after d term */

        /* Loop over all possible c values. */
        idx_t c_max = int_sqrt(rem1 / 4ULL);
        for (idx_t c = 0; c <= c_max; ++c) {
            idx_t c2   = c * c;
            idx_t rem2 = rem1 - 4ULL * c2;     /* remaining after c term */

            /* Loop over all possible b values. */
            idx_t b_max = int_sqrt(rem2 / 2ULL);
            for (idx_t b = 0; b <= b_max; ++b) {
                idx_t b2   = b * b;
                idx_t rem3 = rem2 - 2ULL * b2; /* remaining after b term */

                /* Check if the remaining can be written as a^2. */
                idx_t a0 = int_sqrt(rem3);
                if (a0 * a0 == rem3) {
                    /* Count sign choices: 2 for each non–zero variable, 1 for zero. */
                    seq_t mult = 1;
                    if (a0 != 0) mult *= 2;
                    if (b  != 0) mult *= 2;
                    if (c  != 0) mult *= 2;
                    if (d  != 0) mult *= 2;
                    result += mult;
                }
            }
        }
    }
    return result;
}
