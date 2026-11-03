/*
 * A349215  a(n) = Sum_{d|n} d^c(d), where c is the prime characteristic
 *          (A010051).
 * https://oeis.org/A349215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) + Σ_{p|n} (p−1) (sum over distinct prime divisors p
 *            of n)
 * Range:     u8 n <= 256, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 9844 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A349215 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1)=1; return 0 for n==0 just in case */
    if (n == 0) return 0;

    idx_t m = n;                /* remaining part to factor */
    seq_t tau = 1;              /* number of divisors τ(n) */
    seq_t prime_sum = 0;        /* Σ(p-1) over distinct prime divisors */

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* count exponent of p */
            idx_t exp = 0;
            do {
                m /= p;
                ++exp;
            } while (m % p == 0);

            tau *= (seq_t)(exp + 1);      /* update τ(n) */
            prime_sum += (seq_t)(p - 1);  /* add (p-1) */
        }
    }

    /* if m > 1, it is a prime factor larger than sqrt(original n) */
    if (m > 1) {
        tau *= 2;                      /* exponent 1 → (1+1) */
        prime_sum += (seq_t)(m - 1);
    }

    return tau + prime_sum;
}
