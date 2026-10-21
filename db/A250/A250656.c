/*
 * A250656  T(n,k)=Number of (n+1)X(k+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the values of T(m,k) (see the table in the OEIS
 *            * entry) along the antidiagonals of the matrix with indices m,k
 *            ≥ 1. * For a given sequence index n, we first determine the
 *            diagonal s = m + k, * then compute the corresponding row and
 *            column (m = offset, k = s‑offset), * and finally...
 * Range:     u8 n <= 19, u16 n <= 103, u32 n <= 463, u64 n <= 880,
 *            u128 n <= 880, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the smallest s ≥ 2 such that s*(s‑1)/2 ≥ n. */
    idx_t low = 2, high = 2;
    while (( (__int128)high * (high - 1) ) / 2 < n) {
        if (high > (ULLONG_MAX >> 1)) { high = ULLONG_MAX; break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__int128)mid * (mid - 1) ) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;   /* diagonal index (m + k) */

    /* Count of elements before this diagonal. */
    __int128 prev_cnt = ( (__int128)(s - 2) * (s - 1) ) / 2;
    idx_t offset = (idx_t)( (__int128)n - prev_cnt );  /* 1‑based position in diagonal */
    idx_t m = offset;                  /* row (n index) */
    idx_t k = s - offset;              /* column (k index) */

    /* Evaluate T(m,k) using the closed‑form formula. */
    seq_t pow2_m_minus1 = pow2(m - 1);       /* 2^(m‑1) */
    seq_t pow2_m_plus1  = pow2(m + 1);       /* 2^(m+1) */

    seq_t k_sq = (seq_t)k * (seq_t)k;
    seq_t term1 = pow2_m_minus1 * k_sq;
    seq_t term2 = ((seq_t)5 * pow2_m_minus1 - (seq_t)1) * (seq_t)k;

    return term1 + term2 + pow2_m_plus1;
}
