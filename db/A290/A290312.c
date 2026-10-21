/*
 * A290312  Third diagonal sequence of the Sheffer triangle A094816 (special
 *          Charlier).
 * https://oeis.org/A290312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24 + 70*n + 69*n^2 + 26*n^3 + 3*n^4) / 24
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 266, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote index to seq_t for all calculations */
    const seq_t N = (seq_t)n;
    /* Compute powers of N */
    seq_t N2 = N * N;          /* n^2 */
    seq_t N3 = N2 * N;         /* n^3 */
    seq_t N4 = N3 * N;         /* n^4 */

    /* Evaluate the numerator of the formula */
    seq_t num = 3 * N4;        /* 3*n^4 */
    num += 26 * N3;            /* +26*n^3 */
    num += 69 * N2;            /* +69*n^2 */
    num += 70 * N;             /* +70*n   */
    num += 24;                 /* +24     */

    /* Divide by 24 (exact division for all integers n) */
    return num / 24U;
}
