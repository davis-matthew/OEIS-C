/*
 * A365499  Dirichlet g.f.: zeta(s) * Product_{p prime} (1 + 1/p^s -
 *          1/p^(4*s)).
 * https://oeis.org/A365499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A365499: a(n) = ∏_{p^e‖n} (2 if e ≤ 3 else 1)
 *               = 2^{# of prime powers of n with exponent ≤ 3}
 * This counts the unitary divisors of n that are biquadrate‑free.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1, and 0 is not queried */
        return 1;

    seq_t  res = 1;
    idx_t  m   = n;
    idx_t  exp = 0;

    /* process factor 2 */
    while ((m & 1ULL) == 0) {
        exp++;
        m >>= 1;
    }
    if (exp <= 3 && exp > 0)
        res *= (seq_t)2;

    /* process odd prime factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }
            if (exp <= 3)
                res *= (seq_t)2;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1)  /* exponent = 1 ≤ 3 */
        res *= (seq_t)2;

    return res;
}
