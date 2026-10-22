/*
 * A345345  a(n) = Sum_{d^2|n} omega(n/d^2).
 * https://oeis.org/A345345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d^2|n} omega(n / d^2) Computed via factorization: if
 *            n = Π p_i^{e_i}, let A_i = floor(e_i/2) + 1, B_i =
 *            floor((e_i-1)/2) + 1. Then a(n) = Σ_i B_i * (∏_{j≠i} A_j)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
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
    /* prime factors and exponents of n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    if (cnt == 0)                  /* n = 1 */
        return 0;

    seq_t A[64], B[64];
    for (size_t i = 0; i < cnt; ++i) {
        A[i] = (seq_t)(ex[i] / 2 + 1);
        B[i] = (seq_t)((ex[i] - 1) / 2 + 1);
    }

    /* product of all A[i] */
    seq_t P = 1;
    for (size_t i = 0; i < cnt; ++i)
        P *= A[i];

    /* sum of B[i] * product of other A[j] */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += B[i] * (P / A[i]);

    return sum;
}
