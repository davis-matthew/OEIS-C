/*
 * A385043  The sum of the unitary divisors of n whose exponents in their
 *          prime factorizations are all powers of 2 (A138302).
 * https://oeis.org/A385043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} (p^e + 1) if e is a power of two, otherwise
 *            factor contributes 1
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m == 1) return (seq_t)1;

    /* prime factors and exponents (n <= 1e4, far below 64) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor 2 first */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) { m >>= 1; ++e; }
        primes[cnt] = 2ULL;
        exps[cnt]  = e;
        ++cnt;
    }

    /* odd primes */
    for (uint64_t p = 3ULL; p <= m / p; p += 2ULL) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]  = e;
            ++cnt;
        }
    }

    if (m > 1) {          /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* contribute only if e is a power of two */
        if (e && ((e & (e - 1ULL)) == 0ULL)) {
            seq_t term = seq_pow((seq_t)primes[i], (idx_t)e);
            term += (seq_t)1;
            result *= term;
        }
    }
    return result;
}
