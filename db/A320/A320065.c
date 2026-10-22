/*
 * A320065  a(n) is the smallest integer i such that binomial(2i,i) > n.
 * https://oeis.org/A320065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer i with binomial(2i, i) > n
 * Range:     u8 n <= 5, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 1 the definition is not given, but we return 1. */
    if (n < 1)
        return (seq_t)1;

    const seq_t threshold = (seq_t)n;

    idx_t low  = 1;
    idx_t high = 1;

    /* Increase high until binomial(2*high, high) > threshold. */
    while (binomial(high * 2, high) <= threshold)
    {
        high *= 2;
    }

    /* Binary search for minimal i. */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (binomial(mid * 2, mid) > threshold)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
