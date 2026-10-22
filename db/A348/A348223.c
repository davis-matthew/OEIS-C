/*
 * A348223  a(n) = Sum_{d|n} (-1)^(sigma(d) - 1).
 * https://oeis.org/A348223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^(sigma(d)-1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sum of divisors σ(n) for a 64‑bit integer n.
   Result is stored in `seq_t`. */
static seq_t sigma_uint64(uint64_t x)
{
    seq_t sigma = 1;
    uint64_t num = x;

    for (uint64_t p = 2; p * p <= num; ++p) {
        if (num % p == 0) {
            seq_t term = 1;          /* Σ_{k=0}^{e} p^k */
            uint64_t pow  = 1;
            while (num % p == 0) {
                num /= p;
                pow *= p;
                term += (seq_t)pow;
            }
            sigma *= term;
        }
    }
    if (num > 1) {                  /* remaining prime factor */
        seq_t term = (seq_t)(1 + num);
        sigma *= term;
    }
    return sigma;
}

/* OEIS A348223 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, guard against it */

    seq_t result = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor i */
            seq_t sigma_i = sigma_uint64((uint64_t)i);
            result += (sigma_i & 1) ? (seq_t)1 : (seq_t)(-1);

            /* paired divisor n/i, if distinct */
            idx_t j = n / i;
            if (j != i) {
                seq_t sigma_j = sigma_uint64((uint64_t)j);
                result += (sigma_j & 1) ? (seq_t)1 : (seq_t)(-1);
            }
        }
    }
    return result;
}
