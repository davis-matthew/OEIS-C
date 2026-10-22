/*
 * A349132  a(n) = Sum_{d|n} psi(d) * A003958(n/d), where A003958 is fully
 *          multiplicative with a(p) = (p-1), and psi is Dedekind psi
 *          function, A001615.
 * https://oeis.org/A349132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} psi(d) * A003958(n/d)
 * Range:     u8 n <= 41, u16 n <= 2309, u32 n <= 9264, u64 n <= 9264,
 *            u128 n <= 9264, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent multiplicative definition: a(p^e) = (p+1)*p^e - p*(p-1)^e */

/* This implementation factors n with trial division (fast for n ≤ 10000),
   then multiplies the prime‑power contributions as above. */
#include <stdint.h>
#include <stddef.h>

static seq_t pow_seq_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for the OEIS offset, harmless */

    uint64_t k = (uint64_t)n;
    seq_t result = 1;

    /* Process all prime factors except possibly the largest one */
    for (uint64_t p = 2; p <= k / p; ++p) {
        if (k % p == 0) {
            idx_t exp = 0;
            while (k % p == 0) {
                k /= p;
                ++exp;
            }
            seq_t p_pow = pow_seq_uint(p, exp);
            seq_t pmin1_pow = pow_seq_uint(p - 1, exp);
            seq_t term = (seq_t)(p + 1) * p_pow - (seq_t)p * pmin1_pow;
            result *= term;
        }
    }

    /* If k > 1 it is a prime factor > sqrt(original n) */
    if (k > 1) {
        uint64_t p = k;
        idx_t exp = 1;            /* its exponent is 1 */
        seq_t p_pow = (seq_t)p;
        seq_t pmin1_pow = (seq_t)(p - 1);
        seq_t term = (seq_t)(p + 1) * p_pow - (seq_t)p * pmin1_pow;
        result *= term;
    }

    return result;
}
