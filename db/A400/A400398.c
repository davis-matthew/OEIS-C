/*
 * A400398  a(n) = Sum_{d|n} gcd(d, n/d) / gcud(d, n/d), where gcud is the
 *          greatest common unitary divisor.
 * https://oeis.org/A400398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} gcd(d, n/d) / gcud(d, n/d)
 * Range:     u8 n <= 7775, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            (multiplicative, a(p^e) given below)               */
#include <stdint.h>

/* Integer power of a seq_t base to a non‑negative idx_t exponent.   */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Contribution of a prime power p^e according to the closed formula. */
static seq_t a_prime_power(uint64_t p, idx_t e)
{
    seq_t p_seq = (seq_t)p;
    if (e & 1) {                     /* e odd  */
        seq_t pow_val = pow_seq(p_seq, (idx_t)((e + 1) / 2));
        seq_t numer   = pow_val - 1;
        seq_t denom   = p_seq - 1;
        seq_t t       = numer / denom;
        return t * 2;
    } else {                          /* e even */
        seq_t pow_val = pow_seq(p_seq, (idx_t)(e / 2));
        seq_t numer   = pow_val - 1;
        seq_t denom   = p_seq - 1;
        seq_t t       = numer / denom;
        return t * 2 + 1;
    }
}

/* Public sequence function.  External linkage as required.          */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* unused offset, return 0 */
    if (n == 1) return (seq_t)1;     /* a(1) = 1 */

    idx_t N = n;
    seq_t result = 1;

    /* factor 2 separately */
    idx_t e = 0;
    while ((N & 1) == 0) {
        N >>= 1;
        ++e;
    }
    if (e) result *= a_prime_power(2ULL, e);

    /* factor odd primes */
    for (idx_t p = 3; p <= N / p; p += 2) {
        if (N % p == 0) {
            idx_t exp = 0;
            while (N % p == 0) {
                N /= p;
                ++exp;
            }
            result *= a_prime_power((uint64_t)p, exp);
        }
    }

    /* remaining prime greater than sqrt(n) */
    if (N > 1) {
        result *= a_prime_power(N, 1);
    }

    return result;
}
