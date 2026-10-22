/*
 * A331591  a(n) is the number of distinct prime factors of A225546(n), or
 *          equally, number of distinct prime factors of A293442(n).
 * https://oeis.org/A331591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct bit positions set in the binary
 *            representations of the exponents in the prime factorization of n
 *            (i.e. the size of the union of the binary supports of all
 *            exponents)
 * Range:     u8 n <= 9806, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Simple trial division factorization (works for all n that fit in uint64_t). */
static size_t factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex) {
    size_t cnt = 0;
    if (n < 2) return 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[cnt] = p;
            ex[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        pr[cnt] = n;
        ex[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    uint64_t x = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fac_cnt = factor_uint64(x, primes, exps);

    uint64_t mask = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        mask |= exps[i];
    }

    unsigned int bits = 0;
    while (mask) {
        mask &= mask - 1;
        ++bits;
    }

    return (seq_t)bits;
}
