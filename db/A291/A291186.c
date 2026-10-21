/*
 * A291186  a(n) = numerator of (pod(n) / tau(n)).
 * https://oeis.org/A291186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (pod(n) / tau(n))
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 71, u64 n <= 239,
 *            u128 n <= 719, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   pod(n) = product of divisors of n = n^{tau/2}          */
/*   a(n)   = n^{tau/2} / gcd(n^{tau/2}, tau)               */
/*   The function uses prime factorisation of n and tau       */
/*   to construct the reduced numerator as a product of prime powers. */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* fast exponentiation for seq_t using binary exponentiation */
static inline seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        if (exp > 1)
            base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* factorise n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute tau = product (exps[i] + 1) */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (idx_t)(exps[i] + 1);
    }

    /* factorise tau */
    uint64_t t_pr[64], t_ex[64];
    size_t t_cnt = 0;
    factor_u64((uint64_t)tau, t_pr, t_ex, &t_cnt);

    /* build the reduced numerator */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        /* exponent of prime p_i in n^{tau/2} */
        idx_t vi = (idx_t)exps[i] * tau;
        vi /= 2;                     /* division is exact */

        /* exponent of p_i in tau */
        idx_t vp = 0;
        for (size_t j = 0; j < t_cnt; ++j) {
            if (primes[i] == t_pr[j]) {
                vp = (idx_t)t_ex[j];
                break;
            }
        }

        /* reduce exponent by common factors */
        idx_t e = vi;
        if (vp < e)
            e = vi - vp;
        else
            e = 0;

        if (e > 0) {
            seq_t base = (seq_t)primes[i];
            seq_t pow_res = power_seq(base, e);
            result = result * pow_res;
        }
    }

    return result;
}
