/*
 * A302479  Number of partitions of n into two distinct nonprime parts.
 * https://oeis.org/A302479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ways to write n as i + j with 1 ≤ i < j, i and
 *            j * both not prime. This matches the OEIS definition of A302479.
 *            * * The implementation uses the supplied prime test * bool
 *            is_prime(idx_t x); (../utils/is_prime.h). A simple O(n) loop is
 *            * sufficient for the required n ≤ 10000
 * Range:     u8 n <= 689, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* A(1) = 0 by definition – the loop below works for all n ≥ 1. */
    seq_t count = (seq_t)0;

    /* i ranges from 1 to ⌊(n‑1)/2⌋.  For n < 2 the limit is 0 and
     * the loop body is skipped. */
    idx_t limit = (n == 0) ? 0 : (n - 1) / 2;
    for (idx_t i = 1; i <= limit; ++i)
    {
        idx_t j = n - i;           /* j > i by construction */
        if (!is_prime(i) && !is_prime(j))
            ++count;
    }

    return count;
}
