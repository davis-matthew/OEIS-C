/*
 * A384920  Tooth numbering system (deciduous), FDI and ISO 3950.
 * https://oeis.org/A384920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, fini, full
 * Formula:   * a(n) = 56‑n for 1 ≤ n ≤ 5 * a(n) = n+55 for 6 ≤ n ≤ 10 * a(n)
 *            = 96‑n for 11 ≤ n ≤ 15 * a(n) = n+55 for 16 ≤ n ≤ 20 * a(n) = 0
 *            (undefined sequence) for n > 20
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 5) return (seq_t)(56 - (seq_t)n);        /* 55,54,53,52,51 */
    if (n <= 10) return (seq_t)(n + 55);           /* 61,62,63,64,65 */
    if (n <= 15) return (seq_t)(96 - (seq_t)n);      /* 85,84,83,82,81 */
    if (n <= 20) return (seq_t)(n + 55);           /* 71,72,73,74,75 */
    return 0;                                    /* undefined beyond 20 */
}
