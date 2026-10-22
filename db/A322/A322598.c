/*
 * A322598  a(n) is the number of unlabeled rank-3 graded lattices with 3
 *          coatoms and n atoms.
 * https://oeis.org/A322598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (3/4)n^2 + (1/3)n + 1/4 ) Equivalent integer form:
 *            a(n) = floor( (9 n^2 + 4 n + 3) / 12 ) . This implements the
 *            sequence A322598 as a closed-form expression
 * Range:     u8 n <= 18, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; for n<1 return 0 (unused) */
        return 0;

    seq_t N = (seq_t)n;           /* promote index to seq_t */
    seq_t val = ( (seq_t)9 * N * N + (seq_t)4 * N + (seq_t)3 ) / (seq_t)12;
    return val;
}
