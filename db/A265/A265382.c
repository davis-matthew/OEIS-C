/*
 * A265382  Total number of ON (black) cells after n iterations of the "Rule
 *          158" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/16*(10*n^2+36*n+16) for n even, a(n) =
 *            1/16*(10*n^2+32*n+22) for n odd
 * Range:     u8 n <= 18, u16 n <= 322, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        return ((seq_t)10 * n * n + (seq_t)36 * n + (seq_t)16) / (seq_t)16;
    } else {
        return ((seq_t)10 * n * n + (seq_t)32 * n + (seq_t)22) / (seq_t)16;
    }
}
