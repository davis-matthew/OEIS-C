/*
 * A386791  Triangle read by rows: T(n, k) = (n - k)/k if k divides n - k else
 *          0 for k > 0, T(n, 0) = 0^n.
 * https://oeis.org/A386791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence is the triangle read by rows.  
 * For rows r ≥ 0 and 0 ≤ k ≤ r:
 *   T(r,0) = 0^r   (defined as 1 when r = 0, otherwise 0)
 *   T(r,k) = (r−k)/k   if k divides (r−k); otherwise 0
 * a(n) is the n‑th element of the list of all T(r,k) with rows read from r = 0 upwards.
 */

seq_t a(idx_t n) {
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* offset of the first element of row r in the linear ordering */
    idx_t offset = (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t k = n - offset; /* column index within the row */

    if (k == 0) {
        /* T(r,0) = 0^r  (1 when r = 0, else 0) */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    idx_t d = r - k; /* difference r−k, always non‑negative */
    if (d % k == 0)
        return (seq_t)(d / k); /* integer division guaranteed */
    else
        return (seq_t)0;
}
