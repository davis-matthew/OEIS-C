/*
 * A299529  Number of Johnson solids with exactly n types of faces.
 * https://oeis.org/A299529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of Johnson solids with exactly n types of faces.
 *            The only non‑zero values are: a(1) = 5 (solids with a single
 *            face type), a(2) = 35, a(3) = 35, a(4) = 17, and a(n) = 0 for
 *            all other n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)35;
    if (n == 3) return (seq_t)35;
    if (n == 4) return (seq_t)17;
    return (seq_t)0;
}
