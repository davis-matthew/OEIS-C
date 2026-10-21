/*
 * A274637  Square array read by antidiagonals: T(m,n) (m>=0, n>=0) =
 *          (2^floor(m/2)-1)*(2^ceiling(n/2)+1).
 * https://oeis.org/A274637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where T(m,k) = (2^floor(m/2)-1)*(2^ceil(k/2)+1) is
 *            read by antidiagonals. The antidiagonal coordinates (m,k) for
 *            index n are obtained from the smallest s with triangular(s) ≤ n
 *            < triangular(s+1); then k = n - triangular(s) and m = s - k
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t triangular(idx_t x)
{
    return x * (x + 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t s = 0;
    while (triangular(s + 1) <= n) ++s;

    idx_t k = n - triangular(s);     /* offset within the diagonal s */
    idx_t m = s - k;                  /* row coordinate */

    idx_t m_half = m >> 1;            /* floor(m/2)     */
    idx_t k_half = (k + 1) >> 1;      /* ceil(k/2)      */

    seq_t term1 = pow2(m_half) - 1;   /* 2^floor(m/2)-1 */
    seq_t term2 = pow2(k_half) + 1;   /* 2^ceil(k/2)+1  */

    return term1 * term2;
}
