/*
 * A249113  Take n and successively add 1, 2, ..., a(n) until reaching a prime
 *          for the third time.
 * https://oeis.org/A249113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smallest k such that n + T_k is prime and among n +
 *            T_j for 1 ≤ j < k exactly two primes are encountered.
 *            Implementation: iteratively add triangular numbers and test
 *            primality until the third prime is found
 * Range:     u8 n <= 14, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* safety for offset */
    seq_t sn = (seq_t)n;                    /* convert index to seq_t */
    seq_t t  = 0;                           /* current triangular value T_j */
    idx_t j = 1;                            /* step counter (k candidate) */
    unsigned char primes_seen = 0;          /* count of primes observed */

    for (;; ++j) {
        t += (seq_t)j;                      /* T_j = T_{j-1} + j */
        seq_t s = sn + t;                  /* n + T_j */
        if (is_prime_big(s)) {
            ++primes_seen;
            if (primes_seen == 3)          /* third prime reached */
                return (seq_t)j;           /* a(n) = k */
        }
    }
}
