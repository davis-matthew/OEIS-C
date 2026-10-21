/*
 * A288247  2 * smallest possible area of a simple n-sided lattice polygon
 *          whose vertex coordinates x and y are both independent permutations
 *          of the integers 1 ... n, subject to the condition that none of its
 *          edges are mutually parallel.
 * https://oeis.org/A288247
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-2 for n ≥ 16; otherwise a(3)=3, a(4)=2, a(5)=3, a(6)=8,
 *            a(7)=9, a(8)=7, a(9)=9, a(10)=8, a(11)=11, a(12)=12, a(13)=12,
 *            a(14)=13, a(15)=14
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t small[13] = {
        3, // a(3)
        2, // a(4)
        3, // a(5)
        8, // a(6)
        9, // a(7)
        7, // a(8)
        9, // a(9)
        8, // a(10)
        11, // a(11)
        12, // a(12)
        12, // a(13)
        13, // a(14)
        14  // a(15)
    };

    if (n <= 15)
        return small[n - 3];
    return (seq_t)n - 2;
}
