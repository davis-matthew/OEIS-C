/*
 * A376214  a(n) = A379342(A379343(n)).
 * https://oeis.org/A376214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the nth element of the triangular array defined by
    T(r,k) = A000384(r-1) + P(r,k)
    where 1 ≤ k ≤ 4r-3,  r = row number,
    m = 2r – 1, and
       P(r,k) = k                         if k < m  and k odd
              = -k + m + 1                 if k < m  and k even
              = k                         if k ≥ m  and k odd
              = -k + 3m – 1                if k ≥ m  and k even
    The array is read by rows, yielding a permutation of the
    positive integers.  The function below computes a(n) for any
    n ≥ 1 by locating the appropriate row via binary search and
    evaluating the formula above.  No external tables are used.  */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that S(r-1) < n ≤ S(r), where
       S(r) = r(2r-1) = sum_{j=1}^r (4j-3).  */
    idx_t low = 1;
    idx_t high = n;                       /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 s = (__int128)mid * (2 * (__int128)mid - 1);
        if (s >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                        /* row number */

    /* Previous cumulative length S(r-1) */
    __int128 prev = (__int128)(r - 1) * (2 * (__int128)(r - 1) - 1);

    /* Column index within the row (1‑based) */
    idx_t kcol = n - prev;                /* 1 ≤ kcol ≤ 4r-3 */

    /* Base value: A000384(r-1) = (r-1)(2r-3) */
    __int128 base = (__int128)(r - 1) * (2 * (__int128)r - 3);

    /* Parameters for P(r,k) */
    idx_t m = 2 * r - 1;                  /* m = 2r-1 */
    int64_t kk = (int64_t)kcol;
    int64_t mm = (int64_t)m;

    int64_t P;
    if (kk < mm) {
        if (kk & 1LL)                /* kk odd */
            P = kk;
        else                         /* kk even */
            P = -kk + mm + 1;
    } else {
        if (kk & 1LL)                /* kk odd */
            P = kk;
        else                         /* kk even */
            P = -kk + 3 * mm - 1;
    }

    __int128 value = base + P;           /* final term a(n) */

    return (seq_t)value;                 /* cast to the build’s seq_t type */
}
