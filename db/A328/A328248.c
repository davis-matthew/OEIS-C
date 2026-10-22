/*
 * A328248  a(n) = 1 if n is a squarefree number (A005117), otherwise a(n) = 1
 *          + number of iterations of arithmetic derivative (A003415) needed
 *          to reach a squarefree number, or 0 if no such number is ever
 *          reached.
 * https://oeis.org/A328248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is square‑free; otherwise a(n) = 1 + the number of
 *            iterations of the arithmetic derivative needed to reach a
 *            square‑free number; a(n) = 0 if a square‑free number is never
 *            reached. In particular, if n contains a prime factor p with
 *            exponent e ≥ p, then a(n) = 0 (e.g., a(4*n)=0, a(27*...
 * Range:     u8 n <= 548, u16 n <= 8788, u32 n <= 8788, u64 n <= 8788,
 *            u128 n <= 8788, big any n
 * Helpers:   factor_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* external utilities provided by the build */
#include "../utils/is_squarefree.h"
#include "../utils/factor_u64.h"

/* return true if n has a prime factor p with exponent e ≥ p */
static bool has_exp_ge_prime(uint64_t n)
{
    if (n <= 1) return false;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] >= primes[i]) return true;
    return false;
}

/* compute a(n) as defined above */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, not used by OEIS */

    /* if the number already satisfies the zero rule */
    if (has_exp_ge_prime((uint64_t)n))
        return (seq_t)0;

    /* square‑free check */
    if (is_squarefree((seq_t)n))
        return (seq_t)1;

    /* otherwise iterate the arithmetic derivative */
    uint64_t cur = (uint64_t)n;
    unsigned int steps = 0;

    while (1)
    {
        /* compute the arithmetic derivative of cur */
        uint64_t primes[64], exps[64];
        size_t cnt;
        factor_u64(cur, primes, exps, &cnt);

        uint64_t der = 0;
        for (size_t i = 0; i < cnt; ++i)
            der += exps[i] * (cur / primes[i]);

        ++steps;

        /* zero condition for the new number */
        if (has_exp_ge_prime(der))
            return (seq_t)0;

        /* if the derivative is square‑free, we are done */
        if (is_squarefree((seq_t)der))
            return (seq_t)(steps + 1);

        /* detect trivial loop (derivative equals previous number) */
        if (der == cur)
            return (seq_t)0;

        cur = der;
    }
}
