/*
 * A337246  Sum of the first coordinates of all pairs of prime divisors of n,
 *          (p,q), such that p <= q.
 * https://oeis.org/A337246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n , q|n , p and q prime , p <= q} p
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* This implementation factors n by simple trial division up to sqrt(n).
   For n ≤ 10 000 the cost is negligible and we avoid external helper
   functions that may not be linked. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t temp = (uint64_t)n;
    uint64_t primes[32];
    unsigned int k = 0;

    /* factor 2 separately */
    if (temp % 2 == 0) {
        primes[k++] = 2;
        while (temp % 2 == 0) temp /= 2;
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            primes[k++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        primes[k++] = temp;
    }

    seq_t sum = 0;
    for (unsigned int i = 0; i < k; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t cnt = (seq_t)(k - i);
        sum += p * cnt;
    }
    return sum;
}
