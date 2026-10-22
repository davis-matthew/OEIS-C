/*
 * A372583  a(n) = (3*n^5 + 5*n^3)/8.
 * https://oeis.org/A372583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^5 + 5*n^3)/8
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 102, u64 n <= 8677,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For small seq_t types (≤ 64‑bit) use 128‑bit arithmetic
       to avoid intermediate overflow.  For larger seq_t types
       (arbitrary precision) rely on seq_t operations directly. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 N  = n;
        unsigned __int128 N2 = N * N;           /* n^2   */
        unsigned __int128 N3 = N2 * N;          /* n^3   */
        unsigned __int128 N5 = N3 * N2;          /* n^5   */
        unsigned __int128 val = (3 * N5 + 5 * N3) / 8;
        return (seq_t)val;
    } else {
        seq_t N  = (seq_t)n;
        seq_t N2 = N * N;           /* n^2   */
        seq_t N3 = N2 * N;          /* n^3   */
        seq_t N5 = N3 * N2;          /* n^5   */
        seq_t num = (seq_t)3 * N5 + (seq_t)5 * N3;
        return num / (seq_t)8;
    }
}
