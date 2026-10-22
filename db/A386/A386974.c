/*
 * A386974  a(n) is the permanent of the symmetric n X n matrix M_n where
 *          M_n(j,k) = n for j = k, M_n(j,n) = n-j, M_n(n,k) = n-k, M_n(j,k) =
 *          0 otherwise.
 * https://oeis.org/A386974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1) * (n+1) * (2*n+1) / 6 (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    // Convert the index to seq_t for precise operations
    seq_t sn = (seq_t)n;
    // Compute n^(n-1) using the provided utility
    seq_t p = big_pow(sn, (seq_t)(n - 1));

    // Multiply by (n+1)
    p *= (sn + 1);
    // Multiply by (2*n+1)
    p *= (sn * 2 + 1);
    // Divide by 6, exact division
    p /= 6;

    return p;
}
