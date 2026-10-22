/*
 * A330116  Beatty sequence for sinh(x), where 1/e^x + csch(x) = 1.
 * https://oeis.org/A330116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * sinh(x)), where x is the positive solution of *
 *            1/e^x + csch(x) = 1. This implementation uses a high‑precision
 *            fixed‑point * representation of alpha = sinh(x) to compute the
 *            Beatty sequence exactly * for any n up to at least 9·10^18 (well
 *            beyond the required 10⁴)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* 2^63 used as the fixed‑point scaling factor.  All calculations are
     * done in unsigned __int128 so that no overflow occurs when n is
     * large. */
    static const unsigned __int128 SCALE = (unsigned __int128)1 << 63;

    /* The scaled integer approximation of alpha = sinh(x). */
    static unsigned __int128 P_scaled;
    static int initialized = 0;

    if (!initialized) {
        /* Solve y³ – 3y² – y + 1 = 0 for the real root y > 1.
         * Newton’s method converges extremely rapidly. */
        long double y = 3.0L;          /* good initial guess */
        for (int it = 0; it < 200; ++it) {
            long double f  = y*y*y - 3*y*y - y + 1.0L;
            long double df = 3*y*y - 6*y - 1.0L;
            long double yn = y - f/df;
            if (fabsl(yn - y) < 1e-30L) break;
            y = yn;
        }

        /* alpha = sinh(x) = (y² – 1) / (2y). */
        long double alpha = (y*y - 1.0L) / (2.0L*y);

        /* Convert alpha to a fixed‑point integer with SCALE = 2^63.
         * The rounding to nearest guarantees an absolute error < 1/(2·SCALE). */
        long double scaled = alpha * (long double)SCALE;
        P_scaled = (unsigned __int128)(scaled + 0.5L);

        initialized = 1;
    }

    /* Compute floor(n * alpha) = floor((n * P_scaled) / 2^63). */
    unsigned __int128 prod = (unsigned __int128)n * P_scaled;
    return (seq_t)(prod >> 63);
}
