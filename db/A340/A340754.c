/*
 * A340754  Number of partitions of n into 4 parts such that the sum of the
 *          smallest two parts and the sum of the largest two parts are
 *          relatively prime.
 * https://oeis.org/A340754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n/4⌋} Σ_{j=k..⌊(n−k)/3⌋} Σ_{i=j..⌊(n−j−k)/2⌋} ⌊1
 *            / gcd(k+j, n−k−j)⌋
 * Range:     u8 n <= 36, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The inner sum counts the number of i values when gcd(k+j, n−k−j)=1, i.e. 1, otherwise 0. */

#include <stdint.h>

/* Euclidean gcd for idx_t (uint64_t). Defined outside to avoid nested function definitions. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t k = 1; k <= n / 4; ++k)
    {
        idx_t max_j = (n - k) / 3;
        for (idx_t j = k; j <= max_j; ++j)
        {
            /* Condition: gcd(k + j, n - k - j) == 1 */
            if (gcd_idx(k + j, n - k - j) != 1)
                continue;

            idx_t upper = (n - k - j) / 2;
            if (upper < j)
                continue;

            /* Count all i from j to upper inclusive. */
            total += (seq_t)(upper - j + 1);
        }
    }

    return total;
}
