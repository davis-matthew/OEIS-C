/*
 * A386553  Number of 2-colorings of an 3 X 3 X 3 grid, up to rotational
 *          symmetry, by the number of black cells.
 * https://oeis.org/A386553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of z^n in 1/24 * [ (1+z)^27 + 8 (1+z)^3
 *            (1+z^3)^8 + 9 (1+z)^3 (1+z^2)^12 + 6 (1+z)^3 (1+z^4)^6 ]
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Binomial coefficient C(n,k) for 0 ≤ n, k ≤ 27 */
static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n + 1 - i) / i;   /* exact division at each step */
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n > 27) return 0;        /* sequence is finite, extend by 0 outside range */

    seq_t sum = 0;

    /* term 1: (1+z)^27 */
    sum += binom_uint(27, n);

    /* term 2: 8 (1+z)^3 (1+z^3)^8 */
    for (idx_t j = 0; j <= 8; ++j) {
        idx_t i = n - 3 * j;
        if (i > 3 || i < 0) continue;
        sum += 8 * binom_uint(3, i) * binom_uint(8, j);
    }

    /* term 3: 9 (1+z)^3 (1+z^2)^12 */
    for (idx_t j = 0; j <= 12; ++j) {
        idx_t i = n - 2 * j;
        if (i > 3 || i < 0) continue;
        sum += 9 * binom_uint(3, i) * binom_uint(12, j);
    }

    /* term 4: 6 (1+z)^3 (1+z^4)^6 */
    for (idx_t j = 0; j <= 6; ++j) {
        idx_t i = n - 4 * j;
        if (i > 3 || i < 0) continue;
        sum += 6 * binom_uint(3, i) * binom_uint(6, j);
    }

    return sum / 24;   /* division is exact */
}
