/*
 * A339399  Pairwise listing of the partitions of k into two parts (s,t), with
 *          0 < s <= t ordered by increasing values of s and where k = 2,3,...
 *          .
 * https://oeis.org/A339399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   cum_floor_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339399: Pairwise listing of the partitions of k into two parts (s,t)
   with 0 < s <= t ordered by increasing s, for k = 2,3,4,...
   a(n) is the first component of the pair in this ordering.
   The following implementation uses the closed‑form formula from the OEIS
   entry, evaluated with integer arithmetic only. */

/* may use standard headers; seq_t and idx_t are defined by the build */
#include <stdint.h>
#include "../utils/cum_floor_half.h"

/* ---------- integer square root for 0 <= n < 2^64 ----------
   returns floor(sqrt(n)) using 128‑bit arithmetic to avoid overflow */
static inline idx_t floor_sqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) return 0;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1 */

    seq_t nseq = (seq_t)n;
    idx_t two_n = n + n;           /* 2*n, fits in idx_t for used range */

    if ((n & 1ULL) == 0) {        /* n even */
        idx_t sqrt1  = floor_sqrt_u64(two_n - 2); /* floor(sqrt(2*n-2)) */
        idx_t sqrt2  = floor_sqrt_u64(two_n - 3); /* floor(sqrt(2*n-3)) */
        idx_t M = (sqrt2 > 0) ? (sqrt2 - 1) : 0;
        seq_t sum = cum_floor_half(M);

        seq_t term1 = sqrt1 + 1;                         /* 1 + sqrt1 */
        seq_t term2 = (nseq - 2 * sum) >> 1;             /* (n - 2*sum)/2 */
        return term1 - term2;
    } else {                                 /* n odd */
        idx_t sqrt2 = floor_sqrt_u64(two_n - 1); /* floor(sqrt(2*n-1)) */
        idx_t M = (sqrt2 > 0) ? (sqrt2 - 1) : 0;
        seq_t sum = cum_floor_half(M);

        seq_t val = (nseq + 1 - 2 * sum) >> 1; /* (n+1 - 2*sum)/2 */
        return val;
    }
}
