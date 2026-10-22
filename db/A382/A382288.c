/*
 * A382288  Number of records in the n-th composition in standard order.
 * https://oeis.org/A382288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of records in the n‑th composition in standard
 *            order
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find m such that 2^(m-1)-1 < n <= 2^m-1      */
    idx_t m   = 64 - __builtin_clzll(n);          /* ceil(log2(n+1))          */
    idx_t prev = ((idx_t)1 << (m - 1)) - 1;       /* compositions before size m */
    idx_t k    = n - prev;                       /* 1‑based index in size m   */
    idx_t comp_bits = k - 1;                     /* encoding of the composition */

    seq_t record   = 0;
    seq_t max_part = 0;
    seq_t part_len = 0;

    for (idx_t pos = 1; pos <= m; ++pos) {
        ++part_len;
        /* separator after position 'pos' ?
           bit index = m-1-pos  (reversed order) */
        int end = (pos == m) || ((comp_bits >> (m - 1 - pos)) & 1ULL);
        if (end) {
            if (part_len > max_part) {
                max_part = part_len;
                ++record;
            }
            part_len = 0;
        }
    }
    return record;
}
