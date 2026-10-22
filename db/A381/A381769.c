/*
 * A381769  a(n) is the area of the largest rectangle that can be formed from
 *          n sticks whose lengths are 1, 2, ..., n.
 * https://oeis.org/A381769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   For n>=5 * T = n(n+1)/2, // total length of all sticks * m =
 *            floor(T/2), // sum of one pair of opposite sides * a(n) =
 *            floor(m/2) * ceil(m/2) * = (m/2) * ((m+1)/2) // integer
 *            divisions * For n < 5 the values are 0, 0, 0, 0, 3 respectively
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 723, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t small[5] = {0, 0, 0, 0, 3};
    if (n < 5)
        return small[n];

    seq_t nn = (seq_t)n;
    seq_t t = nn * (nn + 1) / 2;     // total length
    seq_t m = t / 2;                 // half of the perimeter (integer division)
    seq_t a = (m / 2) * ((m + 1) / 2);
    return a;
}
