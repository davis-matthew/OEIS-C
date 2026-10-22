/*
 * A368915  a(n) = 1 if there is no prime p such that p^p divides the
 *          arithmetic derivative of n, and 0 otherwise.
 * https://oeis.org/A368915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9888, u16 n <= 9888, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A368915:
   a(n) = 1 if no prime p satisfies p^p | D(n), 0 otherwise
   (D(n) = arithmetic derivative of n).  This implementation
   factorises n and D(n); it works for any seq_t that supports
   integer arithmetic. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1)=0 by definition.  For all n>1 compute as described. */
    if (n == 1) return 0;
    if (n == 0) return 0;              /* out of offset, harmless */

    /* factor n using the supplied helper; arrays large enough for
       any n up to 10 000 (more than enough). */
    uint64_t primes_n[64], exps_n[64];
    size_t cnt_n = 0;
    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);

    /* compute D(n) iteratively using the product rule.
       cur_n holds the product of processed prime powers,
       cur_d holds D(cur_n). */
    seq_t cur_n = 1;
    seq_t cur_d = 0;                    /* D(1) = 0 */
    for (size_t i = 0; i < cnt_n; ++i) {
        uint64_t p = primes_n[i];
        uint64_t e = exps_n[i];

        /* compute p^e */
        seq_t pow_p = 1;
        for (uint64_t k = 0; k < e; ++k) {
            pow_p *= (seq_t)p;
        }

        /* D(p^e) = e * p^(e-1) */
        seq_t d_prime = 0;
        if (e > 0) {
            seq_t pow_div = pow_p / (seq_t)p;   /* p^(e-1) */
            d_prime = (seq_t)e * pow_div;
        }

        /* update: D(a * b) = a*D(b) + b*D(a)  */
        seq_t new_n = cur_n * pow_p;
        seq_t new_d = cur_n * d_prime + pow_p * cur_d;

        cur_n = new_n;
        cur_d = new_d;
    }

    if (cur_d == 0) return 0;          /* safety for n=1 (covered above) */

    /* factor D(n); D(n) fits in 64‑bit for n≤10000, safe to cast. */
    uint64_t D = (uint64_t)cur_d;
    uint64_t primes_d[64], exps_d[64];
    size_t cnt_d = 0;
    factor_u64(D, primes_d, exps_d, &cnt_d);

    /* check the p^p condition */
    for (size_t i = 0; i < cnt_d; ++i) {
        if (exps_d[i] >= primes_d[i]) return 0;   /* found p^p | D(n) */
    }
    return 1;                                       /* safe */
}
