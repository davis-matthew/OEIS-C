/*
 * A275789  Least k such that sigma(n) divides Fibonacci(k) (k > 0).
 * https://oeis.org/A275789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that Fibonacci(k) ≡ 0 (mod sigma(n))
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9542 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    if (sigma <= (seq_t)1)
        return (seq_t)1;                     /* sigma = 1 ⇒ Fibonacci(1) = 1 */

    seq_t mod_prev = (seq_t)0 % sigma;        /* F(0) mod sigma */
    seq_t mod_cur  = (seq_t)1 % sigma;        /* F(1) mod sigma */

    for (idx_t k = 1; ; ++k) {
        if (mod_cur == 0)
            return (seq_t)k;                  /* Fibonacci(k) divisible */

        seq_t next = mod_prev + mod_cur;
        next %= sigma;
        mod_prev = mod_cur;
        mod_cur  = next;
    }
}
