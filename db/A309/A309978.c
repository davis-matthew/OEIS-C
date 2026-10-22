/*
 * A309978  a(n) is the number of positive integers k such that there exists a
 *          nonnegative integer m with k + k^m = n.
 * https://oeis.org/A309978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k ≥ 1 : ∃ m ≥ 0 with k + k^m = n}|
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the numbers k for which n = k + k^m for some m ≥ 0.  Since
   only the existence of such an m matters, we break as soon as a
   solution is found for a given k.  For k = 1 the expression is always
   2; we count it only once.  All arithmetic is performed in `seq_t`
   to avoid overflow. */

#include <stdbool.h>

extern seq_t a(idx_t n)
{
    /* For n = 1, there is no positive k, because k + k^0 = k + 1 = n
       would give k = 0, which is not positive. */
    if (n <= 1)
        return 0;

    seq_t count = 0;
    /* Iterate over all possible k from 1 to n−1. */
    for (idx_t k = 1; k < n; ++k) {
        /* Special case k = 1, because k^m is always 1. */
        if (k == 1) {
            if (n == 2)
                ++count;
            /* No need to consider other m, as they give the same value. */
            continue;
        }

        seq_t power = 1;          /* k^0 */
        /* For each exponent m ≥ 0: try to match n. */
        for (;;) {
            seq_t candidate = (seq_t)k + power;
            if (candidate == n) {
                ++count;
                break;   /* k is counted only once */
            }
            if (candidate > n)
                break;   /* further exponents only increase the value */

            /* Prepare for next exponent.  Avoid overflow by checking
               whether power * k would exceed n before actually
               multiplying. */
            if (power > n / k)
                break;
            power *= k;
        }
    }

    return count;
}
