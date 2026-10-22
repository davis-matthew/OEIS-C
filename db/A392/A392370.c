/*
 * A392370  Sum of even-indexed parts of the reversed n-th composition in
 *          standard order.
 * https://oeis.org/A392370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sum of even-indexed parts of the reversed n-th
 *            composition in standard order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Summation is over parts with 1‑based indices 2,4,6,… in the reverse of the composition
   obtained from the standard order bijection described in OEIS A392370. */

seq_t a(idx_t n) {
    /* In the mapping, the n‑th composition corresponds to k = 2*n. */
    idx_t k = n << 1u;        /* 2*n   (n is unsigned) */
    seq_t sum = 0;

    idx_t prev = 0;           /* previous set‑bit position, starting with 0 */
    idx_t pos  = 0;           /* current bit position (LSB = 0) */
    idx_t idx  = 0;           /* index in the difference array, starting at 0 */

    /* Scan bits of k from LSB upward. */
    while (k) {
        if (k & 1u) {
            idx_t diff = pos - prev;   /* difference between consecutive positions */
            if (idx & 1u)              /* odd index => even‑positioned part in reverse */
                sum += (seq_t)diff;
            prev = pos;
            ++idx;
        }
        ++pos;
        k >>= 1u;
    }

    return sum;
}
