/*
 * A302911  Determinant of n X n matrix whose main diagonal consists of the
 *          first n 7-gonal numbers and all other elements are 1's.
 * https://oeis.org/A302911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n+1)=a(n)*n*(5*n+7)/2, n≥1 Implements A302911:
 *            determinant of an n×n matrix with diagonal 7‑gonal numbers and
 *            off‑diagonal ones
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;   /* a(1) = 1, a(0) unused */

    seq_t result = 1;
    for (idx_t k = 1; k < n; ++k) {
        /* Compute the recurrence factor k * (5*k + 7) / 2  */
        seq_t factor = ((seq_t)k * (((seq_t)k * 5) + 7)) / 2;
        result = result * factor;
    }
    return result;
}
