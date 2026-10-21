/*
 * A275485  Number of integer lattice points from an n X n square in R^2
 *          centered at the origin that are closer (measured using the
 *          Euclidean metric) to the origin than to any of the four sides of
 *          the square.
 * https://oeis.org/A275485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (x,y) ∈ ℤ² : |x|,|y| ≤ ⌊n/2⌋ and 4(x² + y²) < n² –
 *            4n·max(|x|,|y|) + 4·max(|x|,|y|)² }|
 * Range:     u8 n <= 33, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts points with integer arithmetic only. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* the offset is 1 – a(1) is defined, but for completeness keep zero safe */
    if (n == 0)
        return 0;

    idx_t m = n >> 1;          /* floor(n/2) – the maximal |x| or |y| in the square */

    /* cast n to signed for intermediate arithmetic that may become negative */
    int64_t N    = (int64_t)n;
    int64_t N_sq = N * N;
    int64_t fourN = N << 2;    /* 4·n, used in the right‑hand side of the inequality */

    int64_t count = 0;  /* use signed 64‑bit to avoid overflow in the sum */

    for (idx_t ax = 0; ax <= m; ++ax)
    {
        int64_t ax_i  = (int64_t)ax;
        int64_t ax_sq = ax_i * ax_i;

        for (idx_t ay = 0; ay <= m; ++ay)
        {
            int64_t ay_i  = (int64_t)ay;
            int64_t ay_sq = ay_i * ay_i;

            /* M = max(|x|,|y|) for the current pair */
            int64_t M = (ax > ay) ? (int64_t)ax : (int64_t)ay;

            /* Inequality: 4(x² + y²) < n² – 4n·M + 4M²  */
            int64_t left  = 4 * (ax_sq + ay_sq);
            int64_t right = N_sq - fourN * M + 4 * M * M;

            if (left < right)
            {
                /* apply symmetry multiplicities:
                   (0,0) → 1, axis points → 2, interior points → 4 */
                if (ax == 0 && ay == 0)
                    count += 1;
                else if (ax == 0 || ay == 0)
                    count += 2;
                else
                    count += 4;
            }
        }
    }

    return (seq_t)count;
}
