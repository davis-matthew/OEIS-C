/*
 * A331954  a(n) is the least positive k such that floor(n/k) is a prime
 *          number.
 * https://oeis.org/A331954
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A331954: find the least positive k such that
   floor(n/k) is a prime number.  The search stops at k = n/2, because
   for k > n/2 we have floor(n/k) = 1, which is not prime. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 2 is guaranteed by the caller. */
    for (idx_t k = 1; k <= n / 2; ++k)
    {
        idx_t v = n / k;          /* floor(n/k) */
        if (is_prime(v))
            return (seq_t)k;
    }
    /* Fallback: the definition guarantees a solution for n ≥ 2, so
       reaching here is impossible; return n to satisfy the compiler. */
    return (seq_t)n;
}
