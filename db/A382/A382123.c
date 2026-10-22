/*
 * A382123  a(n) = sigma(n)*sigma(2*n)/3 for n >= 1.
 * https://oeis.org/A382123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma(n) * sigma(2*n) / 3
 * Range:     u8 n <= 11, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint32_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
    41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83,
    89, 97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173, 179,
    181, 191, 193, 197, 199
};
static const size_t prime_cnt = sizeof primes / sizeof primes[0];

/* fast integer power by repeated squaring */
static inline seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* compute sigma(m) = sum of all positive divisors of m */
static seq_t sigma_of_number(idx_t m)
{
    seq_t result = 1;
    idx_t temp = m;

    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t term = pow_seq((seq_t)p, e + 1);
            term -= 1;
            term /= ((seq_t)p - 1);
            result *= term;
        }
    }
    /* remaining prime factor if any */
    if (temp > 1) {
        seq_t term = pow_seq((seq_t)temp, 2);
        term -= 1;
        term /= ((seq_t)temp - 1);
        result *= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sigma_n  = sigma_of_number(n);
    seq_t sigma_2n = sigma_of_number(2 * n);
    return (sigma_n * sigma_2n) / 3;
}
