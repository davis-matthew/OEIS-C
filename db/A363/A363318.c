/*
 * A363318  Total distance from n to each of its refactorable unitary
 *          divisors.
 * https://oeis.org/A363318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, τ(d)|d, gcd(d,n/d)=1} (n - d)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Treat n == 0 as 0 (unlikely in OEIS indexing). */
    if (n == 0) return 0;

    /* Factor n into distinct prime powers. */
    uint64_t primes[64];
    unsigned int exps[64];
    size_t k = 0;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[k] = p;
            exps[k] = e;
            ++k;
        }
    }
    if (temp > 1) {
        primes[k] = temp;
        exps[k] = 1;
        ++k;
    }

    seq_t sum = 0;
    seq_t N   = (seq_t)n;

    /* Enumerate all unitary divisors: for each prime either take its full power or not. */
    size_t total = (size_t)1 << k;          /* 2^k divisors */
    for (size_t mask = 0; mask < total; ++mask) {
        seq_t d   = 1;
        seq_t tau = 1;

        for (size_t i = 0; i < k; ++i) {
            if (mask & ((size_t)1 << i)) {
                /* multiply d by p^e */
                seq_t pp = 1;
                for (unsigned int j = 0; j < exps[i]; ++j) {
                    pp *= (seq_t)primes[i];
                }
                d *= pp;

                /* multiply tau by (e+1) */
                tau *= (seq_t)(exps[i] + 1);
            }
        }

        /* d is automatically unitary (no common prime with n/d). */
        if (d % tau == 0) {
            sum += (N - d);
        }
    }
    return sum;
}
