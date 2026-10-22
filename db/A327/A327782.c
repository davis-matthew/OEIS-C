/*
 * A327782  Numbers n that cannot be written as a sum of two or more distinct
 *          parts with the largest part dividing n.
 * https://oeis.org/A327782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer n ≥ 1 that cannot be expressed as a sum of
 *            two or more distinct parts with the largest part dividing n. The
 *            test for a number x is: it has no divisor L (2 ≤ L < x) such
 *            that x ≤ L·(L+1)/2
 * Range:     u8 n <= 19, u16 n <= 804, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required standard headers */
#include <stdbool.h>

/* Helper: determine whether x can be represented as required. */
static bool has_representation(idx_t x)
{
    /* x < 6 cannot have a representation (needs at least two parts). */
    if (x < 6) return false;

    /* Enumerate divisors of x, excluding x itself. */
    for (idx_t d = 2; d * d <= x; ++d)
    {
        if (x % d) continue;

        /* First divisor L1 = d */
        idx_t L1 = d;
        if (L1 != x)
        {
            seq_t bound = (seq_t)L1 * (L1 + 1) / 2;
            if ((seq_t)x <= bound) return true;
        }

        /* Complementary divisor L2 = x/d */
        idx_t L2 = x / d;
        if (L2 != d && L2 != x)
        {
            seq_t bound = (seq_t)L2 * (L2 + 1) / 2;
            if ((seq_t)x <= bound) return true;
        }
    }

    return false;
}

seq_t a(idx_t n)
{
    /* We generate the sequence by scanning integers and counting those
       that have no valid representation.  For n up to about 10,000 this
       is comfortably fast. */
    idx_t count = 0;
    idx_t candidate = 1;

    while (1)
    {
        if (!has_representation(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
