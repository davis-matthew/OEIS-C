/*
 * A290109  a(1) = 1; for n > 1, a(n) = x1^(x2^(x3^(x4^...))) where x1, x2,
 *          ... are the exponents of the primes present (listed from the
 *          smallest prime to the largest) in the prime factorization of n.
 * https://oeis.org/A290109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = x1^(x2^(x3^(…))) where x1, x2, … are
 *            the exponents of the primes in the factorization of n, listed in
 *            ascending order of the primes
 * Range:     u8 n <= 1295, u16 n <= 8099, u32 n <= 9448, u64 n <= 9448,
 *            u128 n <= 9448, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent: a(n) = A067029(n) ^ a(A028234(n)). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    uint16_t exps[64];          /* exponents are small, 16‑bit is plenty */
    size_t count = 0;

    /* Trial‑division factorization – primes come in increasing order. */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[count++] = (uint16_t)e;
        }
    }
    if (m > 1) {               /* remaining prime factor */
        exps[count++] = 1;
    }

    /* Evaluate the exponent tower from right to left. */
    seq_t result = (seq_t)exps[count - 1];
    for (int i = (int)count - 2; i >= 0; --i) {
        result = big_pow((seq_t)exps[i], result);
    }
    return result;
}
