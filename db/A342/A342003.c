/*
 * A342003  Maximal exponent in the prime factorization of the arithmetic
 *          derivative of n: a(n) = A051903(A003415(n)).
 * https://oeis.org/A342003
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal exponent in the prime factorization of the
 *            arithmetic derivative of n. This is computed by factoring n,
 *            evaluating its arithmetic derivative D(n) = Σ e·(n/p), and then
 *            taking the largest exponent in the factorization of D(n). The
 *            algorithm runs in O(√n) per call and is suitable for...
 * Range:     u8 n <= 9780, u16 n <= 9780, u32 n <= 9780, u64 n <= 9780,
 *            u128 n <= 9780, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Trial‑division factorization of a 64‑bit number.
   The caller supplies arrays large enough to hold all factors. */
static void factor_uint64(uint64_t n,
                          uint64_t *factors, uint64_t *exps,
                          size_t *cnt)
{
    size_t c = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t e = 0;
            do
            {
                m /= p;
                ++e;
            } while (m % p == 0);
            factors[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (m > 1)          /* remaining prime factor */
    {
        factors[c] = m;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* behaviour below the OEIS offset is
                                   irrelevant                     */

    /* First factor n. */
    uint64_t factors[64], exps[64];
    size_t cnt = 0;
    factor_uint64((uint64_t)n, factors, exps, &cnt);

    /* Evaluate the arithmetic derivative D(n). */
    uint64_t der = 0;
    for (size_t i = 0; i < cnt; ++i)
        der += exps[i] * ((uint64_t)n / factors[i]);

    if (der <= 1)      /* derivative 0 or 1 has no prime factors          */
        return (seq_t)0;

    /* Factor the derivative and find the largest exponent. */
    size_t der_cnt = 0;
    factor_uint64(der, factors, exps, &der_cnt);

    uint64_t max_exp = 0;
    for (size_t i = 0; i < der_cnt; ++i)
        if (exps[i] > max_exp)
            max_exp = exps[i];

    return (seq_t)max_exp;
}
