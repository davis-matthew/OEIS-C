/*
 * A388975  a(n) = A033885(A003961(n)), where A033885(n) = 3*n-sigma(n), and
 *          A003961 is fully multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A388975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*A003961(n) - sigma(A003961(n))
 * Range:     i8 n <= 10, i16 n <= 767, i32 n <= 9538, i64 n <= 9538,
 *            i128 n <= 9538, big any n
 * Helpers:   next_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961 is fully multiplicative with a(p) = nextprime(p) */
/* The implementation factors n, computes m = ∏ nextprime(p)^e, */
/* then evaluates 3*m - sigma(m) where sigma is the sum‑of‑divisors function. */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/pow_int.h"

static seq_t sigma_of(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t num = pow_int((seq_t)p, exp + 1) - 1;
            seq_t den = (seq_t)p - 1;
            seq_t term = num / den;
            result *= term;
        }
    }
    if (temp > 1) { /* prime factor left */
        seq_t num = pow_int((seq_t)temp, 2) - 1;
        seq_t den = (seq_t)temp - 1;
        seq_t term = num / den;
        result *= term;
    }
    return result;
}

static seq_t compute_A003961(idx_t n)
{
    seq_t m = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            uint64_t next_p = next_prime(p);
            seq_t factor = pow_int((seq_t)next_p, exp);
            m *= factor;
        }
    }
    if (temp > 1) {
        uint64_t next_p = next_prime(temp);
        m *= (seq_t)next_p; /* exponent is 1 */
    }

    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but return 0 safely */
    seq_t m = compute_A003961(n);
    seq_t sigma = sigma_of((idx_t)m);
    seq_t result = m * 3 - sigma;
    return result;
}
