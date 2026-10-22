/*
 * A395458  a(n) = n*tau(n) - (n-1)*tau(rad(n)).
 * https://oeis.org/A395458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*tau(n) - (n-1)*tau(rad(n))
 * Range:     u8 n <= 47, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
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
    if (n == 0) return 0;           /* not defined for n < 1, but harmless */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t tau_n = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau_n *= ((seq_t)ex[i] + 1);

    seq_t tau_rad = ((seq_t)1) << cnt;

    return ((seq_t)n) * tau_n - ((seq_t)(n - 1)) * tau_rad;
}
