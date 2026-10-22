/*
 * A362854  The sum of the divisors of n that are both bi-unitary and
 *          exponential.
 * https://oeis.org/A362854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} S(p, e) where S(p, e) = Σ_{d|e} p^d if e is
 *            odd, = (Σ_{d|e} p^d) – p^(e/2) if e is even
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the provided factor_u64 routine to factorize n.  All arithmetic
   is performed with type seq_t; the caller supplies the proper sized
   integer type for the build. */
#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"

/* Fast exponentiation on seq_t.  The exponent is always small (exponent
   of a prime power in n, at most about 64).  seq_t is assumed to support
   +, -, *, /, etc. */
static inline seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        exp >>= 1U;
        if (exp) base *= base;
    }
    return result;
}

/* Compute the sum of the divisors of n that are both bi‑unitary and
   exponential (OEIS A362854). */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;           /* a(1) = 1 */

    /* Factorize n into primes and exponents.  The maximum distinct
       prime count for 64‑bit n is well below 65. */
    uint64_t primes[70];
    uint64_t exps  [70];
    size_t   cnt   = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        seq_t   sum  = 0;

        /* Sum over all divisors d of e: add p^d. */
        for (uint64_t d = 1; d <= e; ++d) {
            if (e % d == 0) {
                sum += pow_seq((seq_t)p, (unsigned int)d);
            }
        }

        /* Subtract p^(e/2) when e is even. */
        if ((e & 1U) == 0) {
            sum -= pow_seq((seq_t)p, (unsigned int)(e / 2U));
        }

        result *= sum;
    }

    return result;
}
