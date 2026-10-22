/*
 * A370884  Irregular triangle read by rows: T(n,k) is the number of unmatched
 *          left parentheses in the k-th string of parentheses of length n,
 *          where strings within a row are in reverse lexicographical order.
 * https://oeis.org/A370884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = number of unmatched left parentheses in the k-th string
 *            of length r in reverse lexicographic order, where r =
 *            floor(log2(n+1)) and k = n - (2^r - 1)
 * Range:     u8 n <= 9209, u16 n <= 9209, u32 n <= 9209, u64 n <= 9209,
 *            u128 n <= 9209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the row r: 2^r <= n+1 < 2^(r+1)  */
    idx_t r = 0;
    idx_t t = n + 1;
    while (t > 1) {
        t >>= 1;
        r++;
    }

    /* start index of this row: 2^r - 1 */
    idx_t start = ((idx_t)1 << r) - 1;
    idx_t offset = n - start;                     /* 0-based position in the row   */
    idx_t idx_asc = ((idx_t)1 << r) - 1 - offset;  /* ascending index inside the row */

    /* Compute unmatched left parentheses (depth) */
    uint64_t depth = 0;
    for (idx_t i = r; i > 0; ) {
        i--;
        if ((idx_asc >> i) & 1) {      /* ')'  -> try to close */
            if (depth > 0) depth--;
        } else {                       /* '('  -> open */
            depth++;
        }
    }

    return (seq_t)depth;
}
