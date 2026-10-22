/*
 * A373000  a(n) = Sum_{k=1..n} sigma( (n/gcd(k,n))^4 ).
 * https://oeis.org/A373000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{k=1..n} sigma((n/ gcd(k,n))^4)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 83, u64 n <= 7087,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Multiplicative with a(p^e) = Σ_{i=0..e} p^(5i) – Σ_{i=0..e-1} p^i) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

static const uint64_t small_primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};
static const size_t small_prime_cnt = sizeof(small_primes) / sizeof(small_primes[0]);

/* Compute a(p^e) using the short‐sum form to avoid huge intermediate powers. */
static inline seq_t prime_power_value(seq_t p, unsigned int e)
{
    /* S1 = Σ_{i=0..e} p^(5i) */
    seq_t sum1 = 1;
    seq_t term = 1;
    seq_t pow5 = seq_pow(p, 5u);
    for (unsigned int i = 1; i <= e; ++i) {
        term *= pow5;
        sum1 += term;
    }

    /* S0 = Σ_{i=0..e-1} p^i */
    seq_t sum0 = 1;
    seq_t powp = p;
    for (unsigned int i = 1; i < e; ++i) {
        sum0 += powp;
        powp *= p;
    }

    return sum1 - sum0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, return 0 for safety */

    seq_t result = 1;
    idx_t m = n;

    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint64_t p = small_primes[i];
        if ((idx_t)p * (idx_t)p > m)
            break;

        unsigned int exp = 0;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }

        if (exp > 0)
            result *= prime_power_value((seq_t)p, exp);
    }

    if (m > 1)                 /* remaining factor is prime */
        result *= prime_power_value((seq_t)m, 1);

    return result;
}
