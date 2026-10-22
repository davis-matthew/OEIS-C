/*
 * A308050  a(n) = n - prevprime(n - 1), where prevprime(n) is the largest
 *          prime < n.
 * https://oeis.org/A308050
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - prevprime(n - 1)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prev_prime.h"

/* Computes a(n) for n >= 4. For n < 4 the value is undefined in the
   OEIS offset, so we return 0 for convenience. */
seq_t a(idx_t n)
{
    if (n < 4) {
        return 0;
    }
    idx_t p = prev_prime(n - 1);
    return (seq_t)n - (seq_t)p;
}
