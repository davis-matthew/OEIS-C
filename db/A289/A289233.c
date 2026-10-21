/*
 * A289233  Largest number of disjoint point triples that can be chosen from
 *          an n X n X n triangular point grid, each point triple forming a 2
 *          X 2 X 2 triangle.
 * https://oeis.org/A289233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n+1)/6) - 1 if n mod 12 ∈ {3,5,6,8} else
 *            floor(n*(n+1)/6)
 * Range:     u8 n <= 15, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(n*(n+1)/6) */
    seq_t prod = (seq_t)n * ((seq_t)n + 1);
    seq_t base = prod / 6;

    /* subtract 1 for the exceptional residues */
    idx_t r = n % 12;
    if (r == 3 || r == 5 || r == 6 || r == 8)
        base -= 1;

    return base;
}
