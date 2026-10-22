/*
 * A379444  a(n) is the difference between the least prime > (n+1)^2 and the
 *          largest prime < n^2, divided by 2.
 * https://oeis.org/A379444
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (next_prime((n+1)^2) - prev_prime(n^2)) / 2
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prev_prime.h"
#include "../utils/next_prime.h"

/* a(n) = difference between the least prime > (n+1)^2 and the largest prime < n^2, divided by 2.
 * Offset: 2. For n < 2 the function returns 0 (unused). */
seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* offset safety, not used for valid input */

    /* Compute n^2 and (n+1)^2.  Values fit in uint64_t for the intended test range. */
    uint64_t nsq  = n * n;
    uint64_t n1sq = (n + 1) * (n + 1);

    /* Find the required primes. */
    uint64_t p2 = prev_prime(nsq);      /* largest prime < n^2   */
    uint64_t p1 = next_prime(n1sq);     /* least prime > (n+1)^2 */

    /* Compute the difference and halve it.  The difference is always even. */
    seq_t diff = (seq_t)p1 - (seq_t)p2;
    return diff / 2;
}
