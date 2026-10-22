/*
 * A347069  Rectangular array (T(n,k)), by downward antidiagonals: T(n,k) =
 *          position of k in the ordering of {h*e^m, h >= 1, 0 <= m <= n}.
 * https://oeis.org/A347069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where (m,k) are identified by the anti‑diagonal
 *            index n. T(m,k) = position of integer k in the ordering of
 *            {h*e^j : h≥1, 1≤j≤m}. The mapping from n to (m,k) is obtained by
 *            reading the infinite array by downward anti‑diagonals. The first
 *            diagonal (d=0) contains only (1,1), the second...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t denom[5] = {
    0ULL,                      /* unused 0 index */
    2718281828ULL,             /* e^1  * 10^9  ≈ 2.718281828e9      */
    7389056099ULL,             /* e^2  * 10^9  ≈ 7.389056099e9      */
    20085536923ULL,            /* e^3  * 10^9  ≈ 2.0085536923e10    */
    54598150033ULL             /* e^4  * 10^9  ≈ 5.4598150033e10    */
};

seq_t a(idx_t n)
{
    /* Find diagonal d (0‑based) such that d(d+1)/2 < n ≤ (d+1)(d+2)/2 */
    uint64_t d = 0;
    while (((d + 1) * (d + 2)) / 2 < n) ++d;

    uint64_t prev = d * (d + 1) / 2;          /* total elements before diag d */
    uint64_t s    = n - prev;                 /* 1‑based offset within diag d */

    uint64_t m = s;                           /* exponent limit (row)   */
    uint64_t k = d + 2 - s;                   /* integer whose rank is needed */

    /* Base rank: k (positions of integers in S_1 only) */
    seq_t res = 1 + (seq_t)(k - 1);

    /* Add contributions from higher exponents up to min(m,4) */
    uint64_t limit = (m < 4) ? m : 4;
    const uint64_t Q = 1000000000ULL;
    for (uint64_t j = 1; j <= limit; ++j) {
        /* floor(k / e^j) = (k * Q) / denom[j]  (integer division) */
        res += (seq_t)((k * Q) / denom[j]);
    }

    return res;
}
