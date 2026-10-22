/*
 * A391056  Triangle read by rows: T(n,k) = 9*(q+1)*q/2 + (3*q+2)*(n mod 3) -
 *          3*k + 3 for 1 <= k <= n, where q = floor(n/3).
 * https://oeis.org/A391056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n‑th element of the triangle read by rows, where
 *            T(n,k) = 9*(q+1)*q/2 + (3*q+2)*(n mod 3) - 3*k + 3 with q =
 *            floor(n/3)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 >= n. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if ((unsigned __int128)n > cum)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t row = lo;

    /* Position inside the row (zero‑based). */
    unsigned __int128 prev128 = (unsigned __int128)(row - 1) * row / 2;
    idx_t prev = (idx_t)prev128;
    idx_t offset = n - prev - 1;        /* 0 <= offset < row */
    idx_t k = offset + 1;              /* 1 <= k <= row */

    /* Compute T(row, k). */
    idx_t q = row / 3;
    seq_t qSeq = (seq_t)q;

    seq_t val = 9 * (qSeq + 1) * qSeq / 2;
    val += (3 * qSeq + 2) * ((seq_t)(row % 3));
    val -= 3 * ((seq_t)k);
    val += 3;

    return val;
}
