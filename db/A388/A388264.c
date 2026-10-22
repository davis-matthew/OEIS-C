/*
 * A388264  a(n) = gcd(A276086(n), A276086(sigma(n)-n)), where A276086 is the
 *          primorial base exp-function, and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A388264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A276086(n), A276086(sigma(n)-n)) A276086(n) is the
 *            primorial‑base exponent function. The implementation computes
 *            the gcd directly from the mixed‑radix digits, avoiding overflow
 *            of the full primorial values
 * Range:     u8 n <= 19, u16 n <= 111, u32 n <= 1001, u64 n <= 9191,
 *            u128 n <= 9325, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

/* σ(n) – sum of all divisors of n (including n) */
static seq_t sigma_of(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            seq_t p_pow = 1;
            seq_t term_sum = 1;            /* term_sum = 1 + p + p^2 + ... + p^exp */
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                p_pow *= (seq_t)p;
                term_sum += p_pow;
            }
            result *= term_sum;
        }
    }
    if (temp > 1) {                          /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)temp);
    }
    return result;
}

/* Compute a(n) = gcd(A276086(n), A276086(sigma(n)-n)) */
seq_t a(idx_t n)
{
    const seq_t n_seq = (seq_t)n;
    const seq_t m_seq = sigma_of(n) - n_seq;   /* sigma(n) - n  (proper divisors sum) */

    seq_t result = 1;
    seq_t weight  = 1;
    uint64_t prime = 2;                       /* first prime */

    /* Process primes until weight exceeds both n and m */
    seq_t max_val = (n_seq > m_seq ? n_seq : m_seq);
    while (weight <= max_val) {
        seq_t digit_n = (n_seq / weight) % (seq_t)prime;
        seq_t digit_m = (m_seq / weight) % (seq_t)prime;
        seq_t min_digit = (digit_n < digit_m ? digit_n : digit_m);

        for (seq_t k = 0; k < min_digit; ++k)
            result *= (seq_t)prime;

        weight *= (seq_t)prime;
        prime = next_prime(prime);
    }

    return result;
}
