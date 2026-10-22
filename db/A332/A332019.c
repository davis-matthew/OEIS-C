/*
 * A332019  The number of cells added in the n-th generation of the following
 *          procedure: start by coloring any triangle on the snub square
 *          tiling, then repeatedly color every cell that shares a vertex with
 *          a colored cell.
 * https://oeis.org/A332019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*(n-1) for n > 4; a(1)=1, a(2)=9, a(3)=21, a(4)=35
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* undefined, but safe for n==0 */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)9;
        case 3: return (seq_t)21;
        case 4: return (seq_t)35;
        default: return ((seq_t)12) * ((seq_t)(n - 1));
    }
}
