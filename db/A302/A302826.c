/*
 * A302826  a(n) is number of primes of form k^2 + n - k for 0 < k < n.
 * https://oeis.org/A302826
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes of the form k^2 + n - k for 1 ≤ k < n
 * Range:     u8 n <= 16, u16 n <= 1676, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"  /* include the prime test for seq_t */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below the offset are irrelevant */

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t val = (seq_t)k * k - k + n;   /* k^2 - k + n */
        if (isPrimeGeneric(val))
            ++count;
    }
    return count;
}
