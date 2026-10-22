/*
 * A376218  Odd exponentially odd numbers.
 * https://oeis.org/A376218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth odd integer whose prime factorisation
 * contains only odd primes and only odd exponents.
 * We enumerate odd integers, test each for the property,
 * and stop when we have found n such numbers. */

#include <stdbool.h>
#include "../utils/good_seq.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                    /* offset safety; not used */
        return (seq_t)0;

    idx_t  count  = 0;
    seq_t  candidate = (seq_t)1;
    seq_t  result   = (seq_t)0;

    while (count < n) {
        if (good_seq(candidate)) {
            ++count;
            result = candidate;
        }
        ++candidate;
    }
    return result;
}
