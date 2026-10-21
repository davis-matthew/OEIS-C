/*
 * A255740  Square array read by antidiagonals upwards: T(n,1) = 1; for k > 1,
 *          T(n,k) = (n-1)*(n-2)^(A000120(k-1)-1) with n >= 1.
 * https://oeis.org/A255740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 543, u32 n <= 5628, u64 n <= 7909,
 *            u128 n <= 7909, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255740: square array read by antidiagonals upwards.
   Definition: T(n,1)=1; for k>1,  T(n,k)=(n-1)*(n-2)^{popcount(k-1)-1}.
   a(n) is the entry of T read by antidiagonals from (1,1), then
   (2,1),(1,2), then (3,1),(2,2),(1,3), etc. */

#include <stdint.h>
#include <stdbool.h>

/* Helper to compute cum(s) = (s-1)*s/2 with 128‑bit precision when available. */
#if defined(__SIZEOF_INT128__)
static inline idx_t cum(idx_t s) {
    __uint128_t v = (__uint128_t)(s-1) * (__uint128_t)s / 2;
    return (idx_t)v;
}
#else
static inline idx_t cum(idx_t s) {
    return (s-1)*s/2;
}
#endif

seq_t a(idx_t n) {
    /* Find diagonal index s such that cum(s-1) < n <= cum(s) */
    idx_t lo = 2, hi = 2;
    while (cum(hi) < n) hi <<= 1;           /* exponential search for upper bound */
    while (lo < hi) {                       /* binary search within bounds */
        idx_t mid = lo + (hi - lo) / 2;
        if (cum(mid) >= n) hi = mid;
        else                lo = mid + 1;
    }
    idx_t s = lo;                           /* diagonal containing the term */

    idx_t cum_prev = cum(s-1);              /* entries before this diagonal */
    idx_t offset   = n - cum_prev;          /* 1‑based position inside diagonal */
    idx_t k_val    = offset;                /* column index */
    idx_t n_val    = s - offset;            /* row index (bottom → top traversal) */

    /* Special cases */
    if (k_val == 1) return (seq_t)1;         /* first column */
    if (n_val == 1) return (seq_t)0;         /* row 1, k>1 gives 0 */

    /* Compute exponent e = popcount(k_val-1)-1 */
    idx_t pop = __builtin_popcountll((uint64_t)(k_val-1));
    idx_t e   = pop - 1;                     /* e >= 0 */

    seq_t base  = (seq_t)(n_val - 1);         /* (n-1) factor */
    seq_t mult  = (seq_t)(n_val - 2);         /* base for power term */
    seq_t pow_res = (seq_t)1;

    for (idx_t i = 0; i < e; ++i)            /* compute (n-2)^e */
        pow_res = pow_res * mult;

    return base * pow_res;
}
