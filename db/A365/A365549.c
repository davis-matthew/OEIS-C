/*
 * A365549  The number of exponentially odd divisors of the square root of the
 *          largest square dividing n.
 * https://oeis.org/A365549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e==1 ? 1 : 2 + ⌊(e-2)/4⌋)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                                                 */
/*  A multiplicative function:                                        */
/*  If n = ∏ p_i^{e_i}, then                                            */
/*     a(n) = ∏ a(p_i^{e_i})                                           */
/*  where a(p^e) = 1  if e = 1 (n is not divisible by p^2),           */
/*            = 2 + floor((e-2)/4) if e ≥ 2.                          */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* n fits in 64‑bit for input */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* Factorise n.  factor_u64 returns the distinct primes in primes[],
       their exponents in exps[], and cnt is the number of factors.  */
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        seq_t   contrib;
        if (e == 1)
            contrib = 1;                       /* squarefree factor */
        else
            contrib = 2 + ((e - 2) / 4);       /* floor division gives the correct value */
        result *= contrib;
    }

    return result;
}
