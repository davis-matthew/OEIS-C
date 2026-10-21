/*
 * A280684  a(n) = number of divisors of the product of the divisors of n.
 * https://oeis.org/A280684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of the product of the divisors of n =
 *            τ(n^{τ(n)/2}) where τ is the divisor-count function
 * Range:     u8 n <= 59, u16 n <= 839, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, but not used */

    /* Factor n and compute τ(n) */
    seq_t tau_n = 1;
    idx_t tmp = n;
    size_t primes[64];
    size_t exps[64];
    size_t cnt = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            size_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            primes[cnt] = p;
            exps[cnt] = exp;
            ++cnt;
            tau_n *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
        tau_n *= 2;
    }

    /* Compute τ(n^{τ(n)/2}) = ∏ (a_i * τ(n)/2 + 1) */
    seq_t tau_p = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i] * tau_n / (seq_t)2;
        tau_p *= (e + 1);
    }

    return tau_p;
}
