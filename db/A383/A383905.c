/*
 * A383905  Square table read by descending antidiagonals where T(n,k) =
 *          binomial(k+2^n-2,k).
 * https://oeis.org/A383905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 59, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A383905:  read the table T(n,k)=binomial(k+2^n-2, k)
   by antidiagonals with increasing n.  Given an index i (offset 0), the
   algorithm determines the diagonal d with d(d+1)/2 ≤ i < (d+1)(d+2)/2,
   then sets n = i - d(d+1)/2   (row inside the diagonal)
   and k = d - n.  The value is computed as T(n,k).
   The implementation uses a multiplicative binomial routine that
   correctly handles 2^n even for large n.
*/

#include <stdint.h>
#include "../utils/pow2.h"

/* Compute T(n,k) = binomial(k + 2^n - 2, k) for indices n,k ≥ 0. */
static seq_t compute_T(idx_t n, idx_t k)
{
    /* Row 0: only the first entry is 1; otherwise 0. */
    if (n == 0) return (k == 0) ? (seq_t)1 : (seq_t)0;

    /* All other rows start with 1 and then positive values. */
    if (k == 0) return (seq_t)1;

    /* Compute upper = k + 2^n - 2 safely in seq_t. */
    seq_t two_pow = pow2(n);          /* 2^n as arbitrary precision */
    seq_t upper   = two_pow + (seq_t)k;
    upper -= 2;                       /* avoid signed wrap on k-2 */

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t term = upper - (seq_t)(k - i);   /* upper - (k-i) */
        result = (result * term) / (seq_t)i;   /* multiplicative binomial */
    }
    return result;
}

/* Main entry point for the sequence. */
seq_t a(idx_t idx)
{
    /* Find diagonal d such that d(d+1)/2 ≤ idx < (d+1)(d+2)/2. */
    idx_t low  = 0;
    idx_t high = (1ULL << 32);            /* safe upper bound for 64‑bit idx */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= idx) low = mid;
        else            high = mid - 1;
    }
    idx_t d = low;

    /* Total elements before this diagonal. */
    __uint128_t tri128 = (__uint128_t)d * (d + 1) / 2;
    idx_t tri = (idx_t)tri128;

    /* Position inside diagonal. */
    idx_t n = idx - tri;      /* row within diagonal */
    idx_t k = d - n;          /* column within diagonal */

    return compute_T(n, k);
}
