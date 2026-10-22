/*
 * A389310  Triangle read by rows: T(n, k) is the smallest m such that for
 *          every red-blue edge-coloring of the graph K_{m} there exists
 *          either a red n-cycle or a blue k-cycle; Ramsey number r(C_n, C_k).
 * https://oeis.org/A389310
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r and k are obtained by reading the triangle
 *            of Ramsey numbers for cycles (OEIS A389310). The triangle is
 *            indexed by rows n ≥ 3, with k ranging from 3 to n: T(n,k) = 6 if
 *            (n,k) = (3,3) or (4,4); T(n,k) = 2 n − 1 if k is odd; T(n,k) =
 *            max(n + k/2 − 1, 2 k − 1) if n is odd and k...
 * Range:     u8 n <= 1277, u16 n <= 1277, u32 n <= 1277, u64 n <= 1277,
 *            u128 n <= 1277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(3) is the first term; values for n < 3 are undefined */
    if (n < 3) return (seq_t)0;

    /* Offset within the sequence starting at a(3) */
    idx_t off = n - 3;

    /* Find the row r (r ≥ 3) such that the cumulative count up to row r
       exceeds off.  The number of terms up to row r is
       C(r) = (r-2)(r-1)/2.                                   */
    idx_t r = 3;
    idx_t c;
    while (1) {
        c = ((r - 2) * (r - 1)) / 2;      /* C(r) */
        if (c > off) break;
        ++r;
    }

    /* Compute position within the row.  The number of terms before row r
       is C(r-1) = ((r-3)(r-2))/2. */
    idx_t before = ((r - 3) * (r - 2)) / 2;
    idx_t pos = off - before;             /* 0‑based position in row */
    idx_t k = 3 + pos;                     /* column index */

    /* Evaluate T(r,k) according to the piecewise definition.             */
    if ((r == 3 && k == 3) || (r == 4 && k == 4)) {
        return (seq_t)6;
    }

    if (k & 1ULL) {                       /* k odd */
        return (seq_t)((seq_t)r * 2U - 1U);
    }

    if (r & 1ULL) {                       /* r odd, k even */
        seq_t v1 = (seq_t)r + (seq_t)(k / 2U) - (seq_t)1U;
        seq_t v2 = 2U * (seq_t)k - (seq_t)1U;
        return v1 > v2 ? v1 : v2;
    }

    /* r even, k even */
    return (seq_t)r + (seq_t)(k / 2U) - (seq_t)1U;
}
