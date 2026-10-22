/*
 * A363705  The minimum irregularity of all maximal 2-degenerate graphs with n
 *          vertices.
 * https://oeis.org/A363705
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n=3, 4 for n=4, 2 for n=5, 6 for n=6, 8 for n>6
 *            (offset 3)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;
    switch (n) {
    case 3: return (seq_t)0;
    case 4: return (seq_t)4;
    case 5: return (seq_t)2;
    case 6: return (seq_t)6;
    default: return (seq_t)8;
    }
}
