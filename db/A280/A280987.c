/*
 * A280987  {Concatenation n, n-1, n-2, ...3,2,1} mod sigma(n).
 * https://oeis.org/A280987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000422(n) mod A000203(n)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000422(n) = concatenation of n, n-1, ..., 1          */
/* A000203(n) = sigma(n) = sum of divisors of n          */
#include <stdint.h>
#include <stddef.h>

static uint64_t sigma_of_n(uint64_t n)
{
    uint64_t m = n;
    uint64_t result = 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t term = 1;
            uint64_t pp   = 1;
            while (m % p == 0) {
                m /= p;
                pp *= p;
                term += pp;
            }
            result *= term;
        }
    }
    if (m > 1) {
        /* m is prime at this point */
        result *= (m + 1);
    }
    return result;
}

static unsigned num_digits_uint64(uint64_t x)
{
    unsigned d = 0;
    do {
        ++d;
        x /= 10;
    } while (x > 0);
    return d;
}

static uint64_t pow10_mod(uint64_t exp, uint64_t mod)
{
    uint64_t r = 1 % mod;
    for (uint64_t i = 0; i < exp; ++i) {
        r = (r * 10) % mod;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */
    uint64_t nn = (uint64_t)n;
    uint64_t sigma = sigma_of_n(nn);
    if (sigma == 0) sigma = 1;

    uint64_t res = 0;
    uint64_t i = nn;
    while (i >= 1) {
        unsigned d = num_digits_uint64(i);
        uint64_t pow10 = pow10_mod((uint64_t)d, sigma);
        res = (res * pow10 + i) % sigma;
        if (i == 1) break;
        --i;
    }
    return (seq_t)res;
}
