/*
 * A247142  Numbers for which the root mean square of proper divisors is an
 *          integer.
 * https://oeis.org/A247142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer (≥2) whose proper divisors’
 *            root‑mean‑square is an integer
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 59, u64 n <= 1249,
 *            u128 n <= 1249, big any n
 * Helpers:   factor_u64, floor_sqrt, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/floor_sqrt.h"

/* Check whether val satisfies the RMS‑integer property. */
static bool _is_rms_integer(seq_t val)
{
    /* val is at least 2 for the sequence we generate. */
    uint64_t v64 = (uint64_t)val;

    /* Factorization */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(v64, primes, exps, &cnt);

    /* Compute τ(n) and Σ d² over all divisors n. */
    seq_t tau = 1;
    seq_t sigma_sq = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        size_t e = (size_t)exps[i];
        tau = tau * (seq_t)(e + 1);

        seq_t p_sq = p * p;
        seq_t num = seq_pow(p_sq, (idx_t)(e + 1));
        num = num - 1;                     /* p^{2(e+1)} – 1 */
        seq_t denom = p_sq - 1;             /* p² – 1 */
        seq_t term = num / denom;           /* (p^{2(e+1)} –1)/(p²–1) */
        sigma_sq = sigma_sq * term;         /* product over primes */
    }

    /* Sum of squares over proper divisors */
    seq_t val_sq = val * val;
    seq_t proper_sq = sigma_sq - val_sq;    /* subtract n² */

    seq_t k = tau - 1;                      /* number of proper divisors */
    if (k == 0) return false;               /* safety for n<2 */

    /* Must be divisible and quotient a perfect square. */
    if (proper_sq % k != 0) return false;
    seq_t q = proper_sq / k;
    seq_t root = floor_sqrt(q);
    return root * root == q;
}

seq_t a(idx_t n)
{
    seq_t candidate = 2; /* first term is 2 */
    idx_t found = 0;

    while (found < n)
    {
        if (_is_rms_integer(candidate))
        {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
    return candidate; /* unreachable */
}
