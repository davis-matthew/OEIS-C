/*
 * A355034  a(n) is the least base b >= 2 where the sum of digits of n is a
 *          prime number.
 * https://oeis.org/A355034
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least base b ≥ 2 such that the sum of digits of n in
 *            base b is a prime number
 * Range:     u8 n <= 1284, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   primecheck
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/primecheck.h"

/* Returns the least base b ≥ 2 where the digit sum of n in base b
   is a prime.  Defined for n ≥ 2; for n < 2 a 0 is returned. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;         /* sequence starts at n = 2 */
    if (n == 2) return (seq_t)3; /* a(2) = 3 */

    for (idx_t b = 2; b <= n - 1; ++b)
    {
        seq_t t   = (seq_t)n;
        seq_t sum = (seq_t)0;

        while (t != 0)
        {
            sum += t % b;
            t   /= b;
        }

        /* sum is small – fit in uint64_t for the primality test */
        uint64_t s = (uint64_t)sum;
        if (primecheck(s))
            return (seq_t)b;     /* found the least such base */
    }

    /* Should never reach here because base n‑1 always yields sum 2 */
    return (seq_t)(n - 1);
}
