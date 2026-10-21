/*
 * A275254  The bi-unitary gcd-sum function.
 * https://oeis.org/A275254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} bi_unitary_gcd(n,k) where *
 *            bi_unitary_gcd(n,k) = product over primes p dividing n of p^e *
 *            with e = exponent of p in n, included only if the exponent of p
 *            * in k equals e; otherwise a 1 contributes for that prime
 * Range:     u8 n <= 65, u16 n <= 6005, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* factor n; use up to 64 distinct prime factors – enough for n <= 2^64 */
    uint64_t primes[64];
    uint8_t  exps[64];
    int pf_cnt = 0;

    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt]  = e;
            ++pf_cnt;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        primes[pf_cnt] = temp;
        exps[pf_cnt]  = 1;
        ++pf_cnt;
    }

    /* precompute p^e and representation as seq_t */
    uint64_t pow64[64];
    seq_t    pow_seq[64];
    for (int i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint8_t  e = exps[i];
        uint64_t p_pow = 1;
        for (uint8_t t = 0; t < e; ++t) p_pow *= p;
        pow64[i] = p_pow;
        pow_seq[i] = (seq_t)p_pow;
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = 1;
        for (int i = 0; i < pf_cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t p_pow = pow64[i];
            if ((k % p_pow) == 0) {
                uint64_t q = k / p_pow;
                if (q % p != 0) {            /* exponent in k equals e */
                    g *= pow_seq[i];
                }
            }
        }
        sum += g;
    }
    return sum;
}
