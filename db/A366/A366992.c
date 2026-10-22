/*
 * A366992  The sum of divisors of n that are not terms of A322448.
 * https://oeis.org/A366992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} (1 + p + Σ_{q ≤ e, q prime} p^q)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* multiplicative function with a(p^e) given by the above formula              */

#include <stdint.h>

static const unsigned int prime_exponents[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61
};
static const unsigned int prime_exponents_len =
    sizeof(prime_exponents) / sizeof(prime_exponents[0]);

/* integer exponentiation for seq_t bases (exp is small and guaranteed not
   to overflow the result type if the final result fits).                     */
static inline seq_t pow_seq_uint64(uint64_t base, unsigned exp)
{
    seq_t res = (seq_t)1;
    seq_t b   = (seq_t)base;
    for (unsigned i = 0; i < exp; ++i)
        res *= b;
    return res;
}

/* value of a(p^e) for a prime p and exponent e ≥ 1.                          */
static inline seq_t a_prime_power(uint64_t p, unsigned e)
{
    seq_t sum = (seq_t)p + (seq_t)1;          /* 1 + p                                   */
    for (unsigned i = 0; i < prime_exponents_len; ++i) {
        unsigned q = prime_exponents[i];
        if (q > e) break;
        sum += pow_seq_uint64(p, q);          /* add p^q for every prime q ≤ e          */
    }
    return sum;
}

/* main sequence function                                                        */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;              /* a(1) = 1                               */

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* handle factor 2 separately for speed */
    unsigned e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0)
        result *= a_prime_power(2ULL, e);

    /* odd prime factors */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= a_prime_power(p, exp);
        }
    }

    /* if a prime factor larger than sqrt(n) remains, its exponent is 1   */
    if (m > 1ULL)
        result *= a_prime_power(m, 1);

    return result;
}
