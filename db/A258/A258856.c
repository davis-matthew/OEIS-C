/*
 * A258856  Sixth pi-based arithmetic derivative of n.
 * https://oeis.org/A258856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sixth iteration of the pi‑based arithmetic derivative
 *            (i.e., a(n) = A258851⁶(n))
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/compute_pi.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

/* pi‑based arithmetic derivative of m
   (m is treated as a 64‑bit integer, the result is a seq_t) */
static seq_t pi_derivative(uint64_t m)
{
    if (m < 2u)                      /* derivative of 0 or 1 is 0 */
        return (seq_t)0;

    /* factorization: primes[], exps[] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);

    seq_t m_seq   = (seq_t)m;   /* the original value as seq_t   */
    seq_t sum     = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p           = (seq_t)primes[i];
        uint64_t exp64    = exps[i];
        int      exp      = (int)exp64;              /* exponent fits in int      */
        seq_t pi_p        = compute_pi(primes[i]);   /* pi(p) as seq_t             */

        /* derivative of the prime power p^exp   */
        seq_t p_pow       = pow_seq(p, exp - 1);          /* p^(exp-1)                 */
        seq_t der_power   = (seq_t)exp * p_pow * pi_p;    /* exp * p^(exp-1) * pi(p)   */

        /* share = m / p^exp (exact division)     */
        seq_t p_pow_all   = pow_seq(p, exp);              /* p^exp                      */
        seq_t share       = m_seq / p_pow_all;            /* integer division           */

        sum += der_power * share;
    }

    return sum;
}

/* Sixth pi‑based arithmetic derivative of n */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    for (int i = 0; i < 6; ++i)
        result = pi_derivative((uint64_t)result);
    return result;
}
