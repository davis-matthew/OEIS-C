/*
 * A371886  The number of multiples (k) of n where the interval contained in
 *          an equal temperament with k*n divisions of the octave (k*n-ET)
 *          that best approximates the just perfect fifth (3:2) is equal to
 *          that of an equal temperament with n divisions.
 * https://oeis.org/A371886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1 / (2 * abs(round(n*log_2(3/2)) - n*log_2(3/2))))
 * Range:     u8 n <= 305, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses long double arithmetic to obtain the required
   precision for n up to about 10 000.  The intermediate result is
   converted to a 64‑bit unsigned integer before being cast to the
   arbitrary‑precision type seq_t. */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    static long double alpha = 0.0L;          /* log2(3/2) */
    if (alpha == 0.0L)
        alpha = log2l(3.0L / 2.0L);

    long double t   = (long double)n * alpha;                /* n*log2(3/2) */
    long long m     = llroundl(t);                          /* nearest integer to t */
    long double diff = fabsl(t - (long double)m);           /* |t - m| */

    /* 1 / (2 * |t - m|)  */
    long double inv = 1.0L / (2.0L * diff);

    /* floor with a tiny epsilon to avoid rounding errors near an integer */
    const long double eps = 1e-18L;
    uint64_t  result64 = (uint64_t)floorl(inv + eps);

    return (seq_t)result64;   /* cast to the arbitrary‑precision integer type */
}
