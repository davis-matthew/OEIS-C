/*
 * A395561  Smallest positive integer k such that n - k is prime and n + k is
 *          composite.
 * https://oeis.org/A395561
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1054, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements OEIS A395561: a(n) = smallest k>0 such that n-k is prime
   and n+k is composite.  The definition is searched via a simple loop.
   This is adequate for n up to about 10 000; the primality tests in the
   utilities are fast enough for these ranges.
*/
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* provides is_prime_small(idx_t) */

/* External linkage: must be non‑static */
seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined – return 0, but this case is
       not required by the specification. */
    if (n < 3) return (seq_t)0;

    for (idx_t k = 1; ; ++k) {
        idx_t n_minus = n - k;
        idx_t n_plus  = n + k;
        if (is_prime_small(n_minus) && !is_prime_small(n_plus))
            return (seq_t)k;
    }
}
