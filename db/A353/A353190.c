/*
 * A353190  a(n) is the (n-1)st odd number minus the sum of the aliquot parts
 *          of n.
 * https://oeis.org/A353190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005408(n-1) - A001065(n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Guarantee n >= 1 as per OEIS offset */
    if (n == 0) return 0;

    idx_t nn = n;

    /* Factor n ----------------------------------------------------------- */
    /* Allocate temporary storage for at most 64 distinct prime factors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)nn, primes, exps, &cnt);

    /* Compute sigma(n) using the factorisation -------------------------- */
    idx_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = (idx_t)primes[i];
        idx_t e = (idx_t)exps[i];

        idx_t term = 1;      /* Sum of powers of p from 0 to e */
        idx_t p_pow = 1;
        for (idx_t k = 1; k <= e; ++k) {
            p_pow *= p;
            term += p_pow;
        }
        sigma *= term;
    }

    /* a(n) = 3*n - 1 - sigma(n) ---------------------------------------- */
    idx_t result = 3 * nn - 1 - sigma;
    return (seq_t)result;
}
