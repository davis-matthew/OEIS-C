/*
 * A378546  a(n) is the sum of the divisors d of n for which A083345(n/d) is
 *          even, where A083345(n) is the numerator of Sum(e/p:
 *          n=Product(p^e)).
 * https://oeis.org/A378546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d where A083345(n/d) is even. A083345(m) is the
 *            numerator of the reduced fraction Σ e/p over the prime power
 *            factorisation p^e of m
 * Range:     u8 n <= 215, u16 n <= 9559, u32 n <= 9559, u64 n <= 9559,
 *            u128 n <= 9559, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* --- helper: parity test for A083345 ------------------------------------- */
static bool a083345_even(idx_t m)
{
    if (m <= 1)
        return true;                 /* numerator is 0 ⇒ even */
    /* factorisation of m */
    static uint64_t pf[32];
    static uint64_t ep[32];
    size_t cnt;
    factor_u64((uint64_t)m, pf, ep, &cnt);

    /* compute D = ∏ p */
    seq_t D = 1;
    for (size_t i = 0; i < cnt; ++i)
        D *= (seq_t)pf[i];

    /* N₀ = Σ eᵢ * (D / pᵢ) */
    seq_t N0 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)ep[i] * (D / pf[i]);
        N0 += term;
    }

    seq_t g = gcd(N0, D);
    seq_t N = N0 / g;
    return (N & 1) == 0;             /* even if least‑significant bit is 0 */
}

/* --- helper: recursive enumeration of divisors --------------------------- */
static void gen_divs(idx_t idx, seq_t cur, idx_t rem,
                     const idx_t *primes, const idx_t *exps,
                     size_t cnt, seq_t *sum)
{
    if (idx == cnt) {
        if (a083345_even(rem))
            *sum += cur;
        return;
    }

    idx_t p   = primes[idx];
    idx_t e   = exps[idx];
    seq_t power = 1;            /* p⁰ */
    idx_t remaining = rem;      /* n / cur */

    for (idx_t f = 0; f <= e; ++f) {
        gen_divs(idx + 1, cur * power, remaining,
                 primes, exps, cnt, sum);
        if (f < e) {
            power     *= (seq_t)p;
            remaining /= p;
        }
    }
}

/* --- public sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    /* factorisation of n */
    static uint64_t pf_n[32];
    static uint64_t ep_n[32];
    size_t cnt;
    factor_u64((uint64_t)n, pf_n, ep_n, &cnt);

    /* convert to idx_t arrays for recursive routine */
    idx_t primes[32];
    idx_t exps[32];
    for (size_t i = 0; i < cnt; ++i) {
        primes[i] = (idx_t)pf_n[i];
        exps[i]   = (idx_t)ep_n[i];
    }

    seq_t sum = 0;
    gen_divs(0, 1, n, primes, exps, cnt, &sum);
    return sum;
}
