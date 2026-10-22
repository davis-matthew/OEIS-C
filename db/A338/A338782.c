/*
 * A338782  The largest e-squarefree e-divisor of n.
 * https://oeis.org/A338782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^{rad(e)} (rad(e) = largest squarefree divisor
 *            of e)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/rad.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* n is at least 1 */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    /* Factor n into prime powers */
    factor_u64(nn, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t rad_exp = rad(ex[i]);                 /* rad(e) as seq_t */
        seq_t powp   = big_pow((seq_t)pr[i], rad_exp); /* p^{rad(e)} */
        result *= powp;
    }
    return result;
}
