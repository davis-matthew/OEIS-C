/*
 * A256263  Triangle read by rows: T(j,k) = 2*k-1 if k is a power of 2,
 *          otherwise, between positions that are powers of 2 we have the
 *          initial terms of A016969, with j>=0, 1<=k<=A011782(j) and T(0,1) =
 *          0.
 * https://oeis.org/A256263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) is given by an irregular triangle: Row j has length L_j = 1
 *            if j==0 else 2^(j-1). For n>0 let j = floor_log2(n)+1. The row
 *            start index is 2^(j-1). Inside the row the column index is k = n
 *            - 2^(j-1) + 1. If k is a power of two: a(n) = 2*k - 1. Otherwise
 *            let m = floor_log2(k), offset = k - 2^m...
 * Range:     u8 n <= 233, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int floor_log2_idx(idx_t v)
{
    int r = 0;
    while (v >>= 1) r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    /* determine row j */
    int j = floor_log2_idx(n) + 1;          /* j >= 1 for n>0 */

    idx_t start = (idx_t)1 << (j - 1);      /* 2^(j-1) */
    idx_t k     = n - start + 1;            /* column index, 1‑based */

    /* power of two? */
    if ((k & (k - 1)) == 0) {
        return (seq_t)k * 2 - 1;
    } else {
        int m     = floor_log2_idx(k);
        idx_t base = (idx_t)1 << m;         /* 2^m */
        idx_t offset = k - base;            /* 1 <= offset <= 2^m-1 */
        return (seq_t)offset * 6 - 1;       /* 6*offset - 1 */
    }
}
