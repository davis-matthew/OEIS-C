/*
 * A362739  The smallest integer with three (not necessarily distinct)
 *          divisors that add to n.
 * https://oeis.org/A362739
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = n*(p-1)/(2p) for n not a power of two, * where p is the
 *            smallest odd prime dividing n. * For n = 2^k, a(n) = 2^(k-1)
 *            (i.e. n/2)
 * Range:     u8 n <= 16, u16 n <= 1491, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t smallest_odd_prime_factor(uint64_t m)
{
    /* m is odd and >= 3 (since we handle powers of two separately) */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0)
            return d;
    }
    /* m itself is prime */
    return m;
}

seq_t a(idx_t n)
{
    /* Handle powers of two */
    if ((n & (n - 1)) == 0) {
        /* n = 2^k, return 2^(k-1) = n/2 */
        return (seq_t)(n >> 1);
    }

    /* Find smallest odd prime factor p of n */
    uint64_t m = n;
    while ((m & 1) == 0)
        m >>= 1;                 /* remove all factors of 2 */

    uint64_t p = smallest_odd_prime_factor(m);

    /* Compute a(n) = n*(p-1)/(2p) */
    seq_t n_seq = (seq_t)n;
    seq_t p_seq = (seq_t)p;
    seq_t numerator = n_seq * (p_seq - 1);
    seq_t denom = 2 * p_seq;
    return numerator / denom;
}
