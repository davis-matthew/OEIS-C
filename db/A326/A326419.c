/*
 * A326419  a(n) is the number of distinct Horadam sequences of period n.
 * https://oeis.org/A326419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = ∑_{k1<k2, lcm(k1,k2)=n} φ(k1)·φ(k2)
 *            + φ(n)·(φ(n)-1)/2
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_of_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/phi_of_divisor.h"

static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Factor n and store its distinct prime factors and exponents.
   primes[20] holds the primes; exps[20] holds the exponents.
   pcnt receives the number of distinct primes found. */
static void factor_uint64(uint64_t n, uint64_t primes[20], int exps[20], size_t *pcnt)
{
    *pcnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if ((tmp % p) == 0) {
            int e = 0;
            while ((tmp % p) == 0) {
                tmp /= p;
                ++e;
            }
            primes[*pcnt] = p;
            exps[*pcnt] = e;
            ++(*pcnt);
        }
    }
    if (tmp > 1) {
        primes[*pcnt] = tmp;
        exps[*pcnt] = 1;
        ++(*pcnt);
    }
}

/* Generate all divisors of n from its prime factorization.
   divisors[] must have at least 2048 slots. 
   dlen is set to the number of divisors found. */
static void generate_divisors(uint64_t n,
                              const uint64_t primes[20], const int exps[20], size_t pcnt,
                              uint64_t divisors[2048], size_t *dlen)
{
    *dlen = 1;
    divisors[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        size_t oldlen = *dlen;
        for (size_t j = 0; j < oldlen; ++j) {
            uint64_t base = divisors[j];
            for (int k = 1; k <= e; ++k) {
                base *= p;
                divisors[(*dlen)++] = base;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Factor n */
    uint64_t primes[20];
    int exps[20];
    size_t pcnt;
    factor_uint64((uint64_t)n, primes, exps, &pcnt);

    /* Enumerate all divisors of n */
    uint64_t divisors[2048];
    size_t dlen;
    generate_divisors((uint64_t)n, primes, exps, pcnt, divisors, &dlen);

    /* φ values for each divisor */
    seq_t phi_vals[2048];
    for (size_t i = 0; i < dlen; ++i) {
        phi_vals[i] = phi_of_divisor(divisors[i], primes, pcnt);
    }

    /* φ(n) */
    seq_t phi_n = phi_vals[dlen - 1];  /* last divisor is n itself */

    /* Sum over unordered pairs (k1<k2) with lcm(k1,k2) = n */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < dlen; ++i) {
        for (size_t j = i + 1; j < dlen; ++j) {
            uint64_t g = gcd_u64(divisors[i], divisors[j]);
            uint64_t lcm = (divisors[i] / g) * divisors[j];
            if (lcm == (uint64_t)n) {
                sum += phi_vals[i] * phi_vals[j];
            }
        }
    }

    /* Add term for (n,n) pair: φ(n)*(φ(n)-1)/2 */
    seq_t add = phi_n * (phi_n - (seq_t)1) / (seq_t)2;

    return sum + add;
}
