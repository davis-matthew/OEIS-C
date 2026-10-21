/*
 * A295658  Multiplicative with a(p^e) = p^max(0,(floor(e/2)-1)).
 * https://oeis.org/A295658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} p^max(0, floor(e/2)-1)
 * Range:     u8 n <= 9367, u16 n <= 9367, u32 n <= 9367, u64 n <= 9367,
 *            u128 n <= 9367, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                     /* a(1) = 1 */

    /* factorization arrays – sufficient for any 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = primes[i];
        int     e    = (int)exps[i];   /* exponent of prime p in n */
        int     out_e = 0;

        if (e >= 4)                /* floor(e/2)-1 for e>=4, else 0 */
            out_e = e / 2 - 1;

        if (out_e > 0)
        {
            seq_t powp = seq_pow((seq_t)p, (idx_t)out_e);
            result     = seq_mul(result, powp);
        }
    }

    return result;
}
