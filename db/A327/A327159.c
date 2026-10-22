/*
 * A327159  Size of the cycle containing n in the map x -> usigma(x)-x or 0 if
 *          n is not a member of any finite cycle. Here usigma is the sum of
 *          unitary divisors of n (A034448).
 * https://oeis.org/A327159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = size of the cycle containing n under f(x) = σ_u(x) – x,
 *            where σ_u(x) is the sum of unitary divisors of x. Return 0 if n
 *            is not contained in any finite cycle
 * Range:     u8 n <= 9329, u16 n <= 9330, u32 n <= 9330, u64 n <= 9330,
 *            u128 n <= 9330, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */

static seq_t sigma_u(uint64_t n)
{
    if (n == 0) return 0;          /* safety: σ_u(0)=0 */
    if (n == 1) return 1;          /* σ_u(1)=1 */

    /* Factor n (at most 64 bits → ≤ 64 prime factors) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* term = p^a + 1 */
        seq_t term = pow_uint(primes[i], exps[i]) + 1;
        result *= term;
    }
    return result;
}

static seq_t f(seq_t x)
{
    if (x == 0) return 0;            /* f(0)=0 as a fixed point */
    return sigma_u((uint64_t)x) - x;
}

/* external definition required by the build */
seq_t a(idx_t n)
{
    seq_t start    = (seq_t)n;
    seq_t tortoise = f(start);
    seq_t hare     = f(f(start));

    /* Detect cycle using Floyd's algorithm */
    while (tortoise != hare)
    {
        tortoise = f(tortoise);
        hare     = f(f(hare));
    }

    /* Find start of the cycle (mu) */
    seq_t mu = 0;
    tortoise = start;
    while (tortoise != hare)
    {
        tortoise = f(tortoise);
        hare     = f(hare);
        mu++;
    }

    /* Find cycle length (lambda) */
    seq_t lambda = 1;
    hare = f(tortoise);
    while (tortoise != hare)
    {
        hare = f(hare);
        lambda++;
    }

    /* If n is in the cycle (mu == 0), return its length; otherwise 0 */
    return (mu == 0) ? lambda : (seq_t)0;
}
