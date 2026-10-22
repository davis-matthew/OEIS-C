/*
 * A348155  a(n) = tau(n)^2 + omega(n)*(sigma(n) - tau(n)).
 * https://oeis.org/A348155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n)^2 + omega(n)*(sigma(n) - tau(n))
 * Range:     u8 n <= 29, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range, return 0 (not used) */

    uint64_t m  = (uint64_t)n;      /* use 64‑bit integer for factorisation */
    uint64_t primes[64], exps[64];
    size_t   cnt = 0;

    factor_u64(m, primes, exps, &cnt);   /* cnt = number of distinct primes */

    seq_t tau   = (seq_t)1;         /* divisor count product( e_i + 1 )      */
    seq_t omega = (seq_t)cnt;       /* number of distinct prime factors      */
    seq_t sigma = (seq_t)1;         /* product of (p_i^(e_i+1)-1)/(p_i-1)    */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* update tau: multiply by (e+1) */
        tau *= (seq_t)(e + 1);

        /* compute sum_{k=0}^{e} p^k  (geometric series) */
        seq_t sum  = (seq_t)1;          /* p^0 */
        seq_t pow  = (seq_t)1;
        for (uint64_t j = 1; j <= e; ++j) {
            pow  *= (seq_t)p;           /* pow = p^j */
            sum  += pow;                /* add to sum */
        }

        /* sigma = sigma * sum */
        sigma *= sum;
    }

    /* a(n) = tau^2 + omega*(sigma - tau) */
    seq_t res = tau * tau + omega * (sigma - tau);
    return res;
}
