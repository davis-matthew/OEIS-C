/*
 * A247973  Least k such that Pi - (4*k+2)/v(2*k+2)^2 < 1/n, where the
 *          sequence v is defined in Comments.
 * https://oeis.org/A247973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the least k such that * Pi - (4*k+2)/v(2*k+2)^2 < 1/n *
 *            where v is defined by v(1)=0, v(2)=1, v(n)=v(n-1)/(n-2)+v(n-2)
 *            and * v(2k+2) = (2k+1)!! / (2^k k!). This implementation
 *            computes the * sequence by iterating k and using the recurrence
 *            * w(k) = v(2k+2), * w(1) = 3/2 and w(k) = w(k-1)*...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, used only if compile-time check required */

static const long double PI_L = 3.14159265358979323846264338327950288419716939937510L;

/*
 * Compute the least k such that Pi - (4*k+2)/v(2*k+2)^2 < 1/n.
 *
 * idx_t n : sequence index (n ≥ 1)
 * Returns : seq_t value a(n)
 */
seq_t a(idx_t n)
{
    /* Handle the pathological case n==0 gracefully; though the OEIS offset
     * is 1, returning 0 keeps the function well-defined for callers that
     * might inadvertently pass 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t k = 1;                     /* current k value                       */
    long double w = 3.0L / 2.0L;      /* w(1) = v(4) = 3/2                     */
    long double denom = w * w;        /* w(k)^2                               */
    long double term = (4.0L * (long double)k + 2.0L) / denom;
    long double D = PI_L - term;      /* current D(k) = Pi - (4k+2)/w^2          */

    /* Convert n to long double once for threshold comparison.        */
    long double inv_n = 1.0L / (long double)n;

    /* Increment k until the inequality holds.  The loop is guaranteed
     * to terminate because D(k) decreases towards zero as k grows.   */
    while (D >= inv_n) {
        ++k;
        /* Update w using the recursive relation.
         * w(k) = w(k-1) * (2k+1)/(2k)   for k >= 2 */
        w *= (2.0L * (long double)k + 1.0L) / (2.0L * (long double)k);
        denom = w * w;                           /* w(k)^2                      */
        term = (4.0L * (long double)k + 2.0L) / denom;
        D = PI_L - term;                          /* new D(k)                     */
    }

    return (seq_t)k;
}
