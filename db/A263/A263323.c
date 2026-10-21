/*
 * A263323  The greater of maximal exponent and maximal prime index in the
 *          prime factorization of n.
 * https://oeis.org/A263323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(A051903(n), A061395(n))
 * Range:     u8 n <= 1618, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return (seq_t)0;

    /* arrays large enough for any factorization of a 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t max_exp = 0;
    uint64_t max_p = 0;

    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > max_exp)
            max_exp = (seq_t)exps[i];
        if (primes[i] > max_p)
            max_p   = primes[i];
    }

    seq_t pi_val = compute_pi((idx_t)max_p);  /* π(max_p) */

    return (pi_val > max_exp) ? pi_val : max_exp;
}
