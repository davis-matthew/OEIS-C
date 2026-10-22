/*
 * A340289  a(n) = Sum_{k=1..floor(n/2)} floor(k^(1/(n-k))*(n-k)^(1/k)).
 * https://oeis.org/A340289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊n/2⌋} floor( k^(1/(n-k)) * (n-k)^(1/k) )
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* sum can safely fit in an unsigned 64‑bit integer for n up to
     * at least 10 000 (the maximal test size).  We cast the final
     * result to seq_t, which is guaranteed to have a constructor
     * (or implicit conversion) from an integral type. */
    uint64_t sum = 0;
    idx_t half = n / 2;
    for (idx_t k = 1; k <= half; ++k) {
        idx_t m = n - k;
        long double t = powl((long double)k, 1.0L / (long double)m) *
                        powl((long double)m, 1.0L / (long double)k);
        /* Use a tiny epsilon to guard against round‑off on integer
         * boundaries; long double provides ~19 decimal digits of
         * precision, far more than needed for the values involved. */
        uint64_t term = (uint64_t)floorl(t + 1e-12L);
        sum += term;
    }
    return (seq_t)sum;
}
