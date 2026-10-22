/*
 * A349507  a(n) is the denominator of n!^(2*n)/(n^n^2).
 * https://oeis.org/A349507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = product over primes p|n of p^{max(0, n^2·v_p(n) –
 *            2n·v_p(n!))}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;                    /* copy of n for factorisation */
    idx_t p;

    /* factorisation by trial division, keeping vp for each prime */
    for (p = 2; p * p <= temp; ++p) {
        if (temp % p != 0)
            continue;
        idx_t vp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++vp;
        }

        /* exponent of p in n! (Legendre's formula) */
        idx_t e = 0;
        idx_t power = p;
        while (power <= n) {
            e += n / power;
            if (power > n / p)          /* avoid overflow */
                break;
            power *= p;
        }

        /* difference of exponents, signed to detect negativity */
        int64_t diff = (int64_t)n * n * vp - (int64_t)2 * n * e;
        if (diff > 0) {
            seq_t term = pow_uint((uint64_t)p, (idx_t)diff);
            result = seq_mul(result, term);
        }
    }

    /* if any prime factor of n remains > sqrt(n) */
    if (temp > 1) {
        idx_t prem = temp;              /* the remaining prime */
        idx_t vp = 1;

        idx_t e = 0;
        idx_t power = prem;
        while (power <= n) {
            e += n / power;
            if (power > n / prem)
                break;
            power *= prem;
        }

        int64_t diff = (int64_t)n * n * vp - (int64_t)2 * n * e;
        if (diff > 0) {
            seq_t term = pow_uint((uint64_t)prem, (idx_t)diff);
            result = seq_mul(result, term);
        }
    }

    return result;
}
