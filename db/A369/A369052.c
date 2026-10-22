/*
 * A369052  a(n) = n - n'*2, where n' is the arithmetic derivative of n,
 *          A003415.
 * https://oeis.org/A369052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - 2*A003415(n) (arithmetic derivative of n)
 * Range:     i8 n <= 47, i16 n <= 4095, i32 n <= 8741, i64 n <= 8741,
 *            i128 n <= 8741, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

static seq_t derivative_u64(uint64_t n)
{
    if (n <= 1)
        return 0;               /* A003415(0) = A003415(1) = 0 */

    uint64_t primes[64];       /* enough for any 64‑bit number */
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t der = 0;
    seq_t nseq = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* term = e * (n / p)  (all are integer) */
        seq_t term = (nseq / (seq_t)primes[i]) * (seq_t)exps[i];
        der += term;
    }
    return der;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n <= 1)
        return nn;               /* a(0)=0, a(1)=1 */

    seq_t der = derivative_u64((uint64_t)n);
    return nn - 2 * der;
}
