/*
 * A362339  a(n) = n! * Sum_{k=0..floor(n/4)} (-k)^k / (k! * (n-4*k)!).
 * https://oeis.org/A362339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (-k)^k / (k! * (n-4*k)!)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for OEIS A362339.
 *
 * The implementation uses only integer arithmetic on the seq_t type,
 * which is defined elsewhere by the build system.  No dynamic
 * allocation is required, and the routine runs in O(n^2) operations
 * that are inexpensive for the typical problem size (n ≤ 10 000).
 */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; k <= n / 4; ++k)
    {
        /*  term = C(n,4k) = n! / ((4k)! (n-4k)!)  */
        seq_t term = 1;
        idx_t fourk = 4 * k;
        /* compute the binomial coefficient multiplicatively */
        for (idx_t i = 1; i <= fourk; ++i)
        {
            term = term * (seq_t)(n - fourk + i) / (seq_t)i;
        }

        /* multiply by (4k)! / k! = product of i = k+1 .. 4k */
        for (idx_t i = k + 1; i <= fourk; ++i)
        {
            term = term * (seq_t)i;
        }

        /* compute (-k)^k = (-1)^k * k^k  */
        seq_t pow = 1;
        for (idx_t i = 0; i < k; ++i)
        {
            pow = pow * (seq_t)k;
        }

        term = term * pow;
        if (k & 1)      /* (-1)^k */
            term = -term;

        sum += term;
    }

    return sum;
}
