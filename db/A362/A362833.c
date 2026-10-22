/*
 * A362833  Number of partitions of n into two distinct parts (s,t) such that
 *          mu(s) = mu(t).
 * https://oeis.org/A362833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/2)} [mu(k)=mu(n-k)]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n <= 2)          /* for n=1 or 2 the loop is empty */
        return 0;

    idx_t half = (n - 1) >> 1;   /* floor((n-1)/2) */
    seq_t count = 0;

    for (idx_t k = 1; k <= half; ++k) {
        seq_t mu1 = mobius(k);
        seq_t mu2 = mobius(n - k);
        if (mu1 == mu2)
            ++count;
    }

    return count;
}
