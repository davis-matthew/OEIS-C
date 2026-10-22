/*
 * A394431  The smallest base b < n where the sum of the digits for the number
 *          n in the base b is the largest, with 1 < b < n and a(1) = a(2) =
 *          1.
 * https://oeis.org/A394431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For each n, return the smallest base b < n (with 1 < b < n) such that the
 * sum of the digits of n in base b, denoted digsum(n,b), is maximal.
 * By definition a(1)=a(2)=1.  For n>2 this function performs an
 * exhaustive search over all admissible bases.  It uses only integer
 * operations and is compatible with both C11 and C++17.  The function
 * automatically handles any idx_t and seq_t type provided by the
 * surrounding project. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n <= 2)
        return (seq_t)1;

    idx_t bestBase = 2;     /* first candidate base */
    seq_t bestSum = 0;       /* best digit sum found so far */

    for (idx_t b = 2; b < n; ++b)
    {
        idx_t temp = n;
        seq_t sum = 0;

        /* Compute the digit sum of n expressed in base b */
        while (temp > 0)
        {
            sum += temp % b;
            temp /= b;
        }

        /* Update best if this base produces a larger sum.
         * If sums are equal, we keep the earlier (smaller) base,
         * because the loop scans in increasing order. */
        if (sum > bestSum)
        {
            bestSum = sum;
            bestBase = b;
        }
    }

    return (seq_t)bestBase;
}
