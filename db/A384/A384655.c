/*
 * A384655  a(n) = Sum_{k=1..n} A051903(gcd(n,k)).
 * https://oeis.org/A384655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A051903(gcd(n,k)) where A051903(m) is the
 *            maximum exponent in the prime factorisation of m (A051903( p^e )
 *            = e). The function below evaluates the sum by summing over all
 *            divisors d of n: a(n) = Σ_{d|n} A051903(d) * φ(n/d)
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursive helper: enumerate all divisors of n
   idx      : current prime index
   p        : array of prime bases
   e        : array of prime exponents
   pcnt     : total number of distinct primes
   max_e    : current maximum exponent among prime exponents chosen for the divisor
   phi_acc  : accumulated φ(n/d) value for the partial divisor
   acc      : pointer to the accumulated result */
static void rec(int idx, const uint64_t *p, const uint64_t *e,
                int pcnt, uint64_t max_e, seq_t phi_acc, seq_t *acc)
{
    if (idx == pcnt) {
        *acc += (seq_t)max_e * phi_acc;
        return;
    }

    uint64_t prime = p[idx];
    uint64_t exp   = e[idx];

    /* Pre‑compute p^k for k = 0..exp to reuse during the recursion */
    seq_t powk[65];
    powk[0] = (seq_t)1;
    for (uint64_t k = 1; k <= exp; ++k)
        powk[k] = powk[k - 1] * (seq_t)prime;

    for (uint64_t d_exp = 0; d_exp <= exp; ++d_exp) {
        uint64_t rem_exp = exp - d_exp;                /* exponent of p in n/d   */
        seq_t phi_part;                                /* φ contribution of this prime   */

        if (rem_exp == 0) {
            phi_part = (seq_t)1;                       /* φ(p^0) = 1                     */
        } else {
            /* φ(p^rem_exp) = (p-1) * p^(rem_exp-1) */
            phi_part = (seq_t)(prime - 1) * powk[rem_exp - 1];
        }

        seq_t new_phi = phi_acc * phi_part;
        uint64_t new_max_e = max_e;
        if (d_exp > new_max_e)
            new_max_e = d_exp;

        rec(idx + 1, p, e, pcnt, new_max_e, new_phi, acc);
    }
}

seq_t a(idx_t n)
{
    if (n < 1)          /* safety, though OEIS offset starts at 1 */
        return 0;

    /* Factor n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t sum = (seq_t)0;
    rec(0, primes, exps, (int)pcnt, (uint64_t)0, (seq_t)1, &sum);
    return sum;
}
