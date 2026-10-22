/*
 * A354114  The smallest number that contains all the digits of n as a
 *          substring but does not equal n.
 * https://oeis.org/A354114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A354114: The smallest number that contains all the digits of
   n as a substring but does not equal n.
   Formula: a(n) = 10*n                        if 10^k <= n <= (10^(k+1)-1)/9
            a(n) = n + 10^(k+1)                otherwise
   where k is the number of digits of n minus one.
*/

#include <stdint.h>
#include "../utils/power10.h"

/* Count decimal digits of an idx_t value (n > 0).  Returns at least 1. */
static inline int digit_count(idx_t n)
{
    int cnt = 0;
    do {
        n /= (idx_t)10;
        ++cnt;
    } while (n);
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)10;        /* a(0) = 10 */

    /* Number of decimal digits of n.  n > 0 here. */
    int d = digit_count(n);
    idx_t k = (idx_t)(d - 1);   /* k = digits - 1 */

    /* Compute 10^(k+1). */
    seq_t pow_kp1 = power10(k + (idx_t)1);

    /* Compute limit_high = (10^(k+1) - 1) / 9. */
    seq_t limit_high = (pow_kp1 - (seq_t)1) / (seq_t)9;

    /* Convert n to seq_t for comparison. */
    seq_t ns = (seq_t)n;

    if (ns <= limit_high)
        return ns * (seq_t)10;   /* Append a trailing zero. */
    else
        return ns + pow_kp1;     /* Prepend a leading one. */
}
