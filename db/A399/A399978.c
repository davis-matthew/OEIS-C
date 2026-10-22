/*
 * A399978  a(n) = omega(n) + sopf(n), where omega(n) is the number of
 *          distinct prime factors of n (A001221) and sopf(n) is the sum of
 *          the distinct primes dividing n (A008472).
 * https://oeis.org/A399978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p | n} (p + 1), a(1) = 0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for n >= 1.  The function is fully
   deterministic and uses only trial division up to sqrt(n). */
seq_t a(idx_t n) {
    if (n <= 1)
        return 0;

    uint64_t m = (uint64_t)n;
    seq_t distinct = 0;
    seq_t sum = 0;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            ++distinct;
            sum += (seq_t)p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1) {          /* remaining prime factor */
        ++distinct;
        sum += (seq_t)m;
    }
    return distinct + sum;
}
