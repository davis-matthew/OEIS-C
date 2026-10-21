/*
 * A287733  First differences of A069497.
 * https://oeis.org/A287733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = T(k_n) - T(k_{n-1}) for n ≥ 1 * where T(k) = k*(k+1)/2
 *            and the indices k_m form the * sequence 0, 3, 8, 11, 12, 15, 20,
 *            23, 24, 27, 32, 35, ... * i.e. k_m = 12*floor(m/4) + d, with d
 *            chosen from * [0, 3, 8, 11] in that order. The zeroth index (k_0
 *            = 0) * corresponds to the triangular number 0
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_k_by_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_k_by_idx.h"

/* Return the triangular number T(k) = k*(k+1)/2 using seq_t arithmetic. */
static inline seq_t triangular(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * (kk + 1) / 2;
}

/* OEIS A287733: first differences of A069497 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* not used; safety guard */

    idx_t idx_n   = n;                  /* n-th entry of T_k sequence (k_1=3) */
    idx_t idx_n1  = idx_n - 1;          /* previous entry (k_0=0 for n=1) */

    seq_t t_n   = triangular(get_k_by_idx(idx_n));
    seq_t t_prev = triangular(get_k_by_idx(idx_n1));

    return t_n - t_prev;
}
