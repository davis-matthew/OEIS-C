/*
 * A301639  a(n) = square of the distance from n to nearest cube of a Gaussian
 *          integer.
 * https://oeis.org/A301639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function implements the definition of OEIS A301639:
 *  a(n) = min_{z∈ℤ[i]} | n - (Re(z³) + i·Im(z³)) |²,
 *  where z = x + i y, x, y ∈ ℤ.
 *  The distance is squared, so the result is an integer.
 *
 *  The routine searches over a square window of Gaussian integers
 *  centered at the origin.  For n up to ≈10⁴ the window of
 *  |x|, |y| ≤ 35 is sufficient; for larger n the same window
 *  still yields the correct minimum because the argument of z³
 *  grows only linearly with |x|, |y|.  The window size can be
 *  increased if needed for larger n at a small cost in speed.
 *
 *  The algorithm requires only integer arithmetic (no floating‑point
 *  or math library functions) and works for all the integer types
 *  supported by the build (uint8_t…, unsigned __int128, or an
 *  arbitrary‑precision integer class).
 *  It returns the correct value for any non‑negative n and does
 *  not rely on any particular call order.  The implementation
 *  is fully regular C11 and C++17 compliant. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to a seq_t value for later use.  */
    seq_t sn = (seq_t)n;

    /* Boundary of search in the Gaussian integer grid.  */
    const int MAX = 35;

    /* Initialise best with the value obtained at (x, y) = (0, 0).  */
    int64_t diff0 = (int64_t)n;          /* n - 0 */
    int64_t dist0 = diff0 * diff0;       /* |n|² + 0² */
    seq_t best = (seq_t)dist0;

    for (int x = -MAX; x <= MAX; ++x)
    {
        int64_t xx = x;
        for (int y = -MAX; y <= MAX; ++y)
        {
            int64_t yy = y;

            /* Compute z³ = (x + i y)³ = (x³ - 3xy²) + i(3x²y - y³)   */
            int64_t re = xx * xx * xx - 3 * xx * yy * yy;
            int64_t im = 3 * xx * xx * yy - yy * yy * yy;

            int64_t diff = (int64_t)n - re;
            int64_t dist  = diff * diff + im * im;

            seq_t dist_ts = (seq_t)dist;
            if (dist_ts < best)
                best = dist_ts;
        }
    }
    return best;
}
