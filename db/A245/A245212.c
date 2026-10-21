/*
 * A245212  a(n) = n * tau(n) - Sum_{(d<n) | n} (d * tau(d)).
 * https://oeis.org/A245212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 * n * tau(n) - Σ_{d|n} (d * tau(d))
 * Range:     i8 n <= 41, i16 n <= 2519, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint8_t */


seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the sequence, but safe */

    /* factor n : primes[] holds distinct prime factors,
       exps[] holds corresponding exponents          */
    idx_t temp   = n;
    idx_t primes[16];
    uint8_t exps[16];
    size_t cnt = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1) {           /* remaining prime factor         */
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* tau(n) = Π (exp_i + 1) */
    seq_t tau_n = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau_n *= (seq_t)(exps[i] + 1);

    /* Σ_{d|n} d * tau(d) = Π Σ_{e=0}^{k_i} p_i^e * (e+1) */
    seq_t sum_divs = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t s   = 0;
        seq_t pow = 1;   /* p_i^e */
        for (uint8_t e = 0; e <= exps[i]; ++e) {
            s   += pow * (seq_t)(e + 1);
            pow *= (seq_t)primes[i];
        }
        sum_divs *= s;
    }

    seq_t result = (seq_t)2 * (seq_t)n * tau_n - sum_divs;
    return result;
}
