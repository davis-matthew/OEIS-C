/*
 * A342693  a(n) = Sum_{d|n} mu(d) * floor(n/d^2).
 * https://oeis.org/A342693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * floor(n/d^2)
 * Range:     u8 n <= 22, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the distinct prime factors of n */
    idx_t temp = n;
    idx_t primes[64];
    size_t pc = 0;

    if ((temp & 1) == 0) {          /* factor 2 */
        primes[pc++] = 2;
        while ((temp & 1) == 0) {
            temp >>= 1;
        }
    }
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            primes[pc++] = p;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    if (temp > 1) {
        primes[pc++] = temp;        /* last prime factor */
    }

    /* Enumerate all square‑free divisors via subsets of the primes */
    seq_t sum = 0;
    size_t subsets = (size_t)1 << pc;
    for (size_t mask = 0; mask < subsets; ++mask) {
        idx_t d = 1;
        size_t bits = 0;
        for (size_t i = 0; i < pc; ++i) {
            if (mask & (1U << i)) {
                d *= primes[i];
                ++bits;
            }
        }

        /* Compute floor(n / d^2) */
        seq_t denom = (seq_t)d * (seq_t)d;
        if (denom == 0) {
            continue;               /* should never happen, safety */
        }
        seq_t term = (seq_t)n / denom;
        if (term == 0) {
            continue;               /* contributes nothing */
        }

        /* mu(d) = (-1)^k where k = number of prime factors in d */
        if ((bits & 1) == 0) {
            sum += term;            /* even number of primes -> +1 */
        } else {
            sum -= term;            /* odd number of primes -> -1 */
        }
    }
    return sum;
}
