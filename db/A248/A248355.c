/*
 * A248355  Least k such that Pi - k*sin(Pi/k) < 1/(2n).
 * https://oeis.org/A248355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k such that Pi - k*sin(Pi/k) < 1/(2n)
 * Range:     u8 n <= 6291, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Use a high‑precision constant for Pi */
    static const long double PI =
        3.141592653589793238462643383279502884L;

    /* Threshold 1/(2n) as long double to avoid rounding */
    const long double thresh = 1.0L / ((long double)2 * (long double)n);

    /* Search for the smallest k */
    for (idx_t k = 1; ; ++k)
    {
        /* Compute Pi - k*sin(Pi/k) with long double precision */
        const long double k_ld = (long double)k;
        const long double val = PI - k_ld * sinl(PI / k_ld);

        /* Once the inequality holds, return k (converted to seq_t) */
        if (val < thresh)
            return (seq_t)(unsigned long long)k;
    }
}
