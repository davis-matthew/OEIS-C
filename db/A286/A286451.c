/*
 * A286451  Compound filter (2-adic valuation of sigma(n) & 2-adic valuation
 *          of n): a(n) = P(A286357(n), A001511(n)), where P(n,k) is sequence
 *          A000027 used as a pairing function, with a(1) = 0 by an explicit
 *          convention.
 * https://oeis.org/A286451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(v2_sigma(n), v2_n(n)) + 1, with a(1) = 0 * where * v2_x
 *            = exponent of 2 in x, * sigma = sum of divisors function, *
 *            P(n,k) = (n + k) * (n + k + 1) / 2 + n (Cantor pairing) * The
 *            sequence is defined for all n ≥ 1 and is independent of the call
 *            * order
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   v2_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/v2_uint.h"

/* --- helper: fast integer power for seq_t -------------------------------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* --- helper: sum of divisors σ(n) --------------------------------------- */
static seq_t sigma_of(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* σ(1) = 1 */

    seq_t sigma = 1;
    idx_t temp = n;

    /* trial division – sufficient for n up to at least 10 000 */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* term = (p^(e+1) - 1) / (p - 1) */
            seq_t num   = pow_seq((seq_t)p, e + 1) - 1;
            seq_t denom = (seq_t)(p - 1);
            seq_t term  = num / denom;
            sigma *= term;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        seq_t term = ((seq_t)temp * (seq_t)temp - 1) / (temp - 1);
        sigma *= term;
    }

    return sigma;
}

/* --- helper: 2‑adic valuation for seq_t --------------------------------- */
static idx_t v2_seq(seq_t x)
{
    idx_t v = 0;
    while (x != 0 && (x & 1) == 0)
    {
        x >>= 1;
        ++v;
    }
    return v;
}

/* --- public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                     /* a(1) = 0 by convention */

    /* 2‑adic valuations */
    const idx_t v2_n   = v2_uint(n);
    const seq_t s      = sigma_of(n);
    const idx_t v2_sig = v2_seq(s);

    /* Cantor pairing P(n,k) = (n + k)(n + k + 1)/2 + n */
    const idx_t sum   = v2_sig + v2_n;
    const seq_t pair  = ((seq_t)sum * (sum + 1)) / 2 + (seq_t)v2_sig;

    return pair + 1;                /* a(n) = P(v2_sigma, v2_n) + 1 */
}
