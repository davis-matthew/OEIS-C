/*
 * A364357  Number of divisors of n of the form 3*k+2 that are at most
 *          sqrt(n).
 * https://oeis.org/A364357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A364357: Number of divisors of n of the form 3*k+2 that are at most sqrt(n).
   Implementation: brute‑force divisor search up to sqrt(n).
   Complexity: O(sqrt(n)) per call, fast enough for n up to 10 000.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0 && d % 3 == 2)
            ++count;
    }
    return count;
}
