/*
 * A350297  Triangle read by rows: T(n,k) = n!*(n-1)^k/k!.
 * https://oeis.org/A350297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 *   T(n,k) = n!*(n-1)^k / k!      (n ≥ 0, 0 ≤ k ≤ n)
 *   a(n) = T(r,k) where r and k are obtained by unrolling the
 *   triangle row‑by‑row: r = largest integer such that r(r+1)/2 ≤ n,
 *   k = n - r(r+1)/2.
 *
 * The function below computes a(n) for any n ≥ 0, without storing the
 * entire triangle.
 *
 * Utilities used:
 *   big_pow(seq_t base, seq_t exp)  (included via "../utils/big_pow.h")
 */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0U;
    while ( ((r + 1U) * (r + 2U)) / 2U <= n )
        ++r;

    /* Number of terms before this row (indices of rows < r). */
    idx_t prev = (r * (r + 1U)) / 2U;

    /* Column index within the row. */
    idx_t k = n - prev;

    /* Special case row 0: T(0,0) = 1. */
    if (r == 0U)
        return (seq_t)1U;

    /* Compute T(r,k) = (r! / k!) * (r-1)^k. */
    seq_t res = (seq_t)1U;
    for (idx_t i = k + 1U; i <= r; ++i)
        res *= (seq_t)i;           /* multiply by i */

    seq_t pow = big_pow( (seq_t)(r - 1U), (seq_t)k );
    res *= pow;

    return res;
}
