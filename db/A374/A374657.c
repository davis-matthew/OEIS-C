/*
 * A374657  a(n) = (1/4) A374656(n).
 * https://oeis.org/A374657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * A374656(n) A374656(n) = ∏_{k=1..n} d(k) d(1) = 3
 *            For k ≥ 2: d(k) = d(k-1) + L_{k-2} where L_m is the m‑th Lucas
 *            number (L_0 = 2, L_1 = 1, L_{m} = L_{m-1} + L_{m-2}) Hence a(n)
 *            = ∏_{k=1..n} d(k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t product = 3;                      /* a(1) = 3 */
    if (n == 1) return product;             /* return for offset 1 */

    /* Lucas numbers: L0 = 2, L1 = 1 */
    seq_t lucas_prev = 2;   /* L0 */
    seq_t lucas_curr = 1;   /* L1 */

    /* d(k) grows as d(k) = d(k-1) + L_{k-2} */
    seq_t d = 3;            /* d(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t diff = lucas_prev;              /* L_{i-2} */
        d = d + diff;                         /* d(i) */
        product = product * d;                /* a(i) */

        /* Advance Lucas numbers */
        seq_t lucas_next = lucas_prev + lucas_curr;
        lucas_prev = lucas_curr;
        lucas_curr = lucas_next;
    }

    return product;
}
