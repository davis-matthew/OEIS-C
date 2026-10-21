/*
 * A296186  Triangle read by rows, T(n,m) = (1/n)*Sum_{i=1..n}
 *          C(n,i-1)*C(n,i)*C(n,m+1-i), T(0,0)=0, m<2n.
 * https://oeis.org/A296186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 63, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows T(n,m) = (1/n)*Σ_{i=1..n} C(n,i-1)C(n,i)C(n,m+1-i)  */
/* Sequence a(n) is obtained by reading the triangle row‑by‑row, i.e. */
/* a(n) = T(r,m) where r is the unique integer with 1+r(r-1) ≤ n < 1+(r+1)r, */
/* and m = n - (1+r(r-1)).                                                    */
#include <stdint.h>
#include "../utils/binomial.h"   /* provides binomial(idx_t n, idx_t k) */

static inline idx_t isqrt_u64(idx_t v) {
    idx_t lo = 0, hi = v;
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    /* Handle the first (trivial) row */
    if (n == 0) return 0;

    /* Compute row index r using the inequality 1 + r(r-1) ≤ n */
    /* r = ⌊(1 + ⌊√(1+4(n−1))⌋)/2⌋                                 */
    idx_t tmp = 1 + 4ULL * (n - 1);
    idx_t r = (1 + isqrt_u64(tmp)) >> 1;   /* divide by 2 */

    idx_t start = 1 + r * (r - 1);           /* first index of row r */
    idx_t m = n - start;                    /* position within the row */

    seq_t sum = 0;
    /* i ranges so that 0 ≤ m+1-i ≤ r   →   i ∈ [max(1, m+1-r) , min(r, m+1)] */
    idx_t i_min = (m + 1 > r) ? (m + 1 - r) : 1;
    idx_t i_max = (m + 1 < r) ? (m + 1) : r;

    for (idx_t i = i_min; i <= i_max; ++i) {
        seq_t a_val = binomial(r, i - 1);
        seq_t b_val = binomial(r, i);
        idx_t k = m + 1 - i;                 /* 0 ≤ k ≤ r by construction */
        seq_t c_val = binomial(r, k);
        /* perform the product with enough precision for 64‑bit builds */
#if defined(__SIZEOF_INT128__) && !defined(_LP64)
        /* For uint64_t instances we safely use __int128 for the product. */
        unsigned __int128 prod = (unsigned __int128)a_val * b_val * c_val;
        sum += (seq_t)(prod / r);
#else
        /* For arbitrary precision or other types just use normal ops */
        seq_t term = a_val * b_val * c_val / r;
        sum += term;
#endif
    }
    return sum;
}
