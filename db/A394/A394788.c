/*
 * A394788  a(n) = number of triples (x, y, z) such that 2*x^2 + y*z = n,
 *          where x, y, z are positive integers satisfying x <= y < z.
 * https://oeis.org/A394788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = |{ (x,y,z) ∈ ℕ³ : x ≤ y < z, 2*x² + y*z = n }|  */
/*  The implementation counts all admissible triples by iterating over
    all possible x and, for each x, over all divisor pairs (y,z) of
    the residual m = n - 2*x².  The search stops when 2*x² > n.
    Safety checks prevent division by zero when computing integer
    square roots.  This runs in O(n^(3/2)) time and is well within
    the limits for n ≤ 10000.                                            */

#include <stdbool.h>
#include <stdint.h>

/* integer square root for unsigned 64‑bit values; returns floor(sqrt(n)) */
/* Safe even for n == 0 (avoids division by zero).                    */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* maximum possible x: 2*x² ≤ n  ⇒  x ≤ sqrt(n/2)                */
    idx_t x_max = isqrt_u64(n / 2);
    for (idx_t x = 1; x <= x_max; ++x) {
        idx_t s = 2 * x * x;                 /* 2*x²          */
        if (s > n) break;                    /* safety guard */
        idx_t m = n - s;                     /* remaining product y*z  */
        if (m == 0) continue;                /* need y*z > 0    */

        idx_t y_start = x;                   /* y ≥ x           */
        idx_t y_max   = isqrt_u64(m);        /* y ≤ sqrt(m)     */
        if (y_start > y_max) continue;       /* no admissible y */

        for (idx_t y = y_start; y <= y_max; ++y) {
            if (m % y == 0) {
                idx_t z = m / y;
                if (z > y) {                  /* strict y < z   */
                    ++count;
                }
            }
        }
    }
    return count;
}
