/*
 * A382680  a(n) = A382499(A380817(n)).
 * https://oeis.org/A382680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5062, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = A382499(A380817(n))
 *  The array T(n,k), row n (1‑based) has length 4n−3.
 *  For 1 ≤ k ≤ 4n−3:
 *      m  = 2n−1
 *      P  =  { m + k           if k < m  and k odd
 *              m − k + 1       if k < m  and k even
 *              k               if k ≥ m  and k odd
 *              k − m           if k ≥ m  and k even }
 *      T(n,k) = 2(n−1)² − (n−1) + P
 *  The function computes a(n) by:
 *      1. find the row r with S(r-1) < n ≤ S(r),
 *         where S(r) = 2r² − r  (total elements up to row r),
 *      2. let k = n − S(r-1)   (1‑based position in that row),
 *      3. evaluate the above formula for T(r,k).
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* should not occur; offset is 1 */

    /* binary search for the row r: smallest r with S(r) >= n  */
    idx_t lo = 1, hi = n;                    /* hi ≥ r for all n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t s = (seq_t)2 * (seq_t)mid * (seq_t)mid - (seq_t)mid;  /* S(mid) */
        if (s >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                            /* row number */

    /* S(r-1) */
    seq_t prevS = (seq_t)2 * (seq_t)(r - 1) * (seq_t)(r - 1) - (seq_t)(r - 1);
    /* 1‑based position inside the row */
    seq_t k = (seq_t)n - prevS;              /* 1 ≤ k ≤ 4r−3 */

    seq_t base = prevS;                      /* 2(r-1)² − (r-1) */
    seq_t m   = (seq_t)(2 * r - 1);          /* 2r−1 */

    seq_t P;
    if (k < m) {                             /* k < m */
        if (k & 1)   /* odd */
            P = m + k;
        else         /* even */
            P = m - k + 1;
    } else {                                 /* k ≥ m */
        if (k & 1)   /* odd */
            P = k;
        else         /* even */
            P = k - m;
    }
    return base + P;
}
