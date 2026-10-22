/*
 * A338170  a(n) is the number of divisors d of n such that tau(d) divides
 *          sigma(d).
 * https://oeis.org/A338170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of divisors d of n such that τ(d) divides
 *            σ(d)
 * Range:     u8 n <= 18, u16 n <= 1753, u32 n <= 9869, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursive helper that counts arithmetic divisors. */
static void
rec(const uint64_t *pr, const uint64_t *ex, size_t num, size_t idx,
    seq_t val, seq_t tau, seq_t sigma, seq_t *cnt)
{
    if (idx == num) {
        if ((sigma % tau) == 0)
            (*cnt)++;
        return;
    }

    uint64_t p   = pr[idx];
    size_t     emax = (size_t)ex[idx];
    seq_t pow_p = 1;          /* p^e for current e */
    for (size_t e = 0; e <= emax; ++e) {
        seq_t pow_p_next = pow_p * (seq_t)p;               /* p^(e+1) */
        seq_t term = (pow_p_next - 1) / ((seq_t)p - 1);    /* contributions to σ */
        seq_t new_val   = val   * pow_p;
        seq_t new_tau   = tau   * (e + 1);
        seq_t new_sigma = sigma * term;
        rec(pr, ex, num, idx + 1, new_val, new_tau, new_sigma, cnt);
        pow_p = pow_p_next;
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;                /* convert to native type */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   cnt = 0;

    factor_u64(N, primes, exps, &cnt);

    seq_t result = 0;
    rec(primes, exps, cnt, 0, (seq_t)1, (seq_t)1, (seq_t)1, &result);
    return result;
}
