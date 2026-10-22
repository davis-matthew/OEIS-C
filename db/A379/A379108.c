/*
 * A379108  Dirichlet convolution of sigma with A359579.
 * https://oeis.org/A379108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} sigma(d) * A359579(n/d) The implementation
 *            evaluates a(n) multiplicatively: - a(2^e) = 2^e - for odd prime
 *            p: * if p is a Mersenne prime (p+1 is a power of two) then
 *            a(p^e) = (p^(e+2) + ((-1)^e-1)*(p-1)/2 - 1)/(p^2-1) (simplified
 *            to (p^(e+2)-1)/(p^2-1) for e even, (p^(e+2)-p)/(p^...
 * Range:     u8 n <= 6, u16 n <= 673, u32 n <= 8985, u64 n <= 8985,
 *            u128 n <= 8985, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Local power function for seq_t */
static inline seq_t seq_pow(seq_t a, unsigned int e)
{
    seq_t res = 1;
    while (e) {
        if (e & 1u) res *= a;
        a *= a;
        e >>= 1;
    }
    return res;
}

/* Test whether p is a Mersenne prime: p+1 is a power of two */
static inline bool is_mersenne_prime(seq_t p)
{
    seq_t x = p + 1;
    return x && ((x & (x - 1)) == 0);
}

/* Factorize n into primes and exponents (uint64_t) */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    uint64_t m = n;
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            e++;
        }
        pr[(*cnt)] = 2;
        ex[(*cnt)] = e;
        (*cnt)++;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            pr[(*cnt)] = p;
            ex[(*cnt)] = e;
            (*cnt)++;
        }
    }
    if (m > 1) {
        pr[(*cnt)] = m;
        ex[(*cnt)] = 1;
        (*cnt)++;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t nn = n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_uint64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        seq_t contrib;

        if (p == 2) {
            /* a(2^e) = 2^e */
            contrib = seq_pow((seq_t)2, (unsigned int)e);
        } else {
            /* Odd prime */
            bool mersenne = is_mersenne_prime(p);
            seq_t p_pow_e1 = seq_pow(p, (unsigned int)(e + 1));
            seq_t p_pow_e2 = p_pow_e1 * p;

            if (mersenne) {
                seq_t numerator;
                if ((e & 1u) == 0) {
                    /* e even */
                    numerator = p_pow_e2 - (seq_t)1;
                } else {
                    /* e odd */
                    numerator = p_pow_e2 - p;
                }
                seq_t denom = p * p - (seq_t)1;
                contrib = numerator / denom;
            } else {
                seq_t numerator = p_pow_e1 - (seq_t)1;
                seq_t denom = p - (seq_t)1;
                contrib = numerator / denom;
            }
        }

        result *= contrib;
    }

    return result;
}
