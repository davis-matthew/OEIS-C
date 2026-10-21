/*
 * A255598  a(n) is the minimal number q>1 such that n(q+1)-1 is prime, or -1
 *          if no such q exists.
 * https://oeis.org/A255598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in OEIS A255598: the smallest integer q>1 such that
   n*(q+1)-1 is prime; returns -1 if no such q exists (theoretically never
   happens for n>=1). */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;          /* offset starts at 1, undefined otherwise */

    seq_t nn = (seq_t)n;
    seq_t q  = 2;                  /* q must be > 1 */

    for (;; ++q) {
        seq_t t = nn * (q + 1) - 1;
        if (is_prime_big(t))
            return q;
    }
}
