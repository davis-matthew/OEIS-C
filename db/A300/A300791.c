/*
 * A300791  Number of ways to write n as x^2 + y^2 + z^2 + w^2, where w is a
 *          positive integer and x,y,z are nonnegative integers for which x or
 *          y or z is a square and (12*x)^2 + (15*y)^2 + (20*z)^2 is also a
 *          square.
 * https://oeis.org/A300791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (x,y,z,w) with * n = x^2 + y^2 + z^2
 *            + w^2, * w > 0, x,y,z ≥ 0, * at least one of x,y,z is a perfect
 *            square, * and (12x)^2 + (15y)^2 + (20z)^2 is a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + n / x) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    /* Quick return for n = 0, although offset starts at 1 */
    if (n == 0) return (seq_t)0;

    /* Precompute array indicating perfect squares up to n */
    bool *is_square = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_square) exit(EXIT_FAILURE);
    for (idx_t i = 0; i * i <= n; ++i) {
        is_square[i * i] = true;
    }

    /* Helper: whether a value ≤ sqrt(n) itself is a perfect square */
    idx_t max_val = isqrt_u64(n);
    bool *is_small_square = (bool*)calloc((size_t)(max_val + 1), sizeof(bool));
    if (!is_small_square) {
        free(is_square);
        exit(EXIT_FAILURE);
    }
    for (idx_t i = 0; i * i <= max_val; ++i) {
        is_small_square[i * i] = true;
    }

    seq_t count = 0;

    for (idx_t x = 0; x <= max_val; ++x) {
        idx_t x2 = x * x;
        idx_t partA = 144 * x2;
        bool x_square = is_small_square[x];

        for (idx_t y = 0; y <= max_val; ++y) {
            idx_t y2 = y * y;
            idx_t partB = 225 * y2;
            bool y_square = is_small_square[y];

            for (idx_t z = 0; z <= max_val; ++z) {
                idx_t z2 = z * z;
                idx_t partC = 400 * z2;
                bool z_square = is_small_square[z];

                /* Check if (12x)^2+(15y)^2+(20z)^2 is square */
                idx_t lhs = partA + partB + partC;
                idx_t sqrt_lhs = isqrt_u64(lhs);
                if (sqrt_lhs * sqrt_lhs != lhs)
                    continue;

                /* At least one of x,y,z must be a square */
                if (!(x_square || y_square || z_square))
                    continue;

                idx_t base_sum = x2 + y2 + z2;
                if (base_sum >= n || !is_square[n - base_sum])
                    continue;   /* w must be positive and integer */

                ++count;
            }
        }
    }

    free(is_square);
    free(is_small_square);
    return (seq_t)count;
}
