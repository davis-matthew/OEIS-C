/*
 * A293056  Rectangular array by antidiagonals: T(n,m) = rank of n*log(2)+m
 *          when all the numbers k*log(2)+h, for k >= 1, h >= 0, are jointly
 *          ranked.
 * https://oeis.org/A293056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,m) = Σ_{k=1}^{n + ⌊m/ log(2)⌋} (m+1 + ⌊(n-k)·log(2)⌋) The
 *            sequence a(n) is obtained by reading T(n,m) by antidiagonals
 * Range:     u8 n <= 171, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   floor_ld
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_ld.h"

seq_t a(idx_t n)
{
    /* --- 1. Convert the sequence index to array coordinates (i,m)  --- */
    idx_t s = 0;
    /* find smallest s such that (s+1)(s+2)/2 >= n   */
    while ((s + 1ULL) * (s + 2ULL) / 2ULL < n) ++s;

    idx_t prev = s * (s + 1ULL) / 2ULL;          /* elements before this diagonal */
    idx_t offset = n - prev - 1ULL;              /* 0‑based offset inside diagonal */
    idx_t i = offset + 1ULL;                    /* row index (n)         */
    idx_t m = s - offset;                       /* column index (m)      */

    /* ----- 2. Pre‑compute constants ------------------------------------- */
    /* log(2) with high precision (long double) */
    static const long double LOG2 = 0.6931471805599453094172321214581765680755L;

    /* floor(m / log(2))  */
    idx_t floor_m_over_log2 = (idx_t)( (long double)m / LOG2 + 1e-15L );

    /* upper bound K for the summation index (k) */
    idx_t K = i + floor_m_over_log2;

    /* ----- 3. Compute the summation Σ floor((i−k)*log(2)) --------------- */
    seq_t sum_floor = 0;
    for (idx_t k = 1; k <= K; ++k) {
        long long a = (long long)i - (long long)k;      /* i−k may be negative */
        long double prod = (long double)a * LOG2;
        long long floor_val = floor_ld(prod);          /* exact floor of product */
        sum_floor += (seq_t)floor_val;
    }

    /* ----- 4. Final formula: T(i,m) = (m+1)*K + Σ floor((i−k)*log(2)) ---- */
    seq_t result = (seq_t)(m + 1ULL) * (seq_t)K + sum_floor;
    return result;
}
