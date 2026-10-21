/*
 * A282914  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 526", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=7, a(3)=11, a(4)=19, a(5)=55,
 *            a(n)=2*a(n-1)+a(n-4)-2*a(n-5) for n>5
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1, 3, 7, 11, 19, 55};

    if (n < 6)
        return init[n];

    seq_t vals[6];
    vals[0] = init[0];
    vals[1] = init[1];
    vals[2] = init[2];
    vals[3] = init[3];
    vals[4] = init[4];
    vals[5] = init[5];

    for (idx_t i = 6; i <= n; ++i) {
        idx_t i1 = (i - 1) % 6;   /* a(i-1) */
        idx_t i4 = (i - 4) % 6;   /* a(i-4) */
        idx_t i5 = (i - 5) % 6;   /* a(i-5) */
        vals[i % 6] = 2 * vals[i1] + vals[i4] - 2 * vals[i5];
    }

    return vals[n % 6];
}
