/*
 * A384919  Tooth numbering system (permanent), FDI and ISO 3950.
 * https://oeis.org/A384919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, fini, full
 * Formula:   a(n) = 18-((n-1)%8) for 1 ≤ n ≤ 8 a(n) = 21+((n-9)%8) for 9 ≤ n
 *            ≤ 16 a(n) = 48-((n-17)%8) for 17 ≤ n ≤ 24 a(n) = 31+((n-25)%8)
 *            for 25 ≤ n ≤ 32 (returns 0 for n > 32) This implements the
 *            two‑row permanent tooth numbering system (FDI and ISO 3950) as
 *            shown in the OEIS entry A384919
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Guard against n < 1: sequence is defined from n = 1 onward. */
    if (n < 1) return 0;

    /* Convert to zero‑based index for easier arithmetic. */
    idx_t z = n - 1;

    /* Determine which 8‑term block the index falls into. */
    idx_t block = z >> 3;          /* divide by 8 */
    idx_t pos   = z & 7;           /* modulo 8 */

    /* Compute the value based on the block. */
    switch (block) {
    case 0:   /* 18 17 16 15 14 13 12 11 */
        return (seq_t)18 - (seq_t)pos;
    case 1:   /* 21 22 23 24 25 26 27 28 */
        return (seq_t)21 + (seq_t)pos;
    case 2:   /* 48 47 46 45 44 43 42 41 */
        return (seq_t)48 - (seq_t)pos;
    case 3:   /* 31 32 33 34 35 36 37 38 */
        return (seq_t)31 + (seq_t)pos;
    default:  /* Out of bounds for this finite sequence */
        return 0;
    }
}
