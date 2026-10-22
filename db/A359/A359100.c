/*
 * A359100  a(n) = (1/4) * Sum_{d|n} phi(5 * d).
 * https://oeis.org/A359100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (1/4) * Sum_{d|n} phi(5*d) The sequence is
 *            multiplicative: a(5^e) = (5^(e+1)-1)/4 a(p^e) = p^e for all
 *            primes p ≠ 5
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          // defined only for n ≥ 1
    seq_t result = 1;
    uint64_t m = n;

    /* small cache for prime factors: n cannot have more than log2(n) distinct primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* trial‑division factorisation */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (m > 1) {                  /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* multiply the contributions of each prime power */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part;

        if (p == 5) {
            /* a(5^e) = (5^(e+1)-1)/4 */
            seq_t pow5 = 1;
            for (uint64_t k = 0; k < e + 1; ++k)
                pow5 *= (seq_t)5;
            part = (pow5 - 1) / 4;
        } else {
            /* a(p^e) = p^e for p ≠ 5 */
            seq_t powp = 1;
            for (uint64_t k = 0; k < e; ++k)
                powp *= (seq_t)p;
            part = powp;
        }
        result *= part;
    }

    return result;
}
