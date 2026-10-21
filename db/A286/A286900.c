/*
 * A286900  Sum of the numbers from n to nextprime(n).
 * https://oeis.org/A286900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = sum_{i=n}^{nextprime(n)} i = (nextprime(n) - n + 1) *
 *            (nextprime(n) + n) / 2
 * Range:     u8 n <= 46, u16 n <= 2476, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

/* External linkage: a(idx_t n) returns seq_t */
seq_t a(idx_t n)
{
    /* Find the next prime greater than n.  next_prime() assumes
       its argument is the starting point, and returns the least
       prime strictly greater than that number. */
    uint64_t p64 = next_prime((uint64_t)n);

    /* Use seq_t for all calculations to avoid overflow in the
       intermediate values.  The cast to seq_t is safe because
       p64 <= UINT64_MAX, which fits in seq_t (seq_t is at least
       64 bits or arbitrary precision). */
    seq_t n64 = (seq_t)n;
    seq_t p64t = (seq_t)p64;

    seq_t count = p64t - n64 + 1;        /* number of terms in the sum */
    seq_t sum   = (count * (n64 + p64t)) / 2;   /* arithmetic progression sum */

    return sum;
}
