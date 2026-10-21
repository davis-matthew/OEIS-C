/*
 * A270085  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 65", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 35*n - 66)/6 for n>3 even a(n) = (4*n^3 + 12*n^2
 *            + 35*n - 69)/6 for n>3 odd Special values: a(0)=1, a(1)=5,
 *            a(2)=10, a(3)=46
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small initial terms */
    static const seq_t init[4] = { (seq_t)1,
                                   (seq_t)5,
                                   (seq_t)10,
                                   (seq_t)46 };
    if (n < 4) return init[n];

    seq_t sn = (seq_t)n;
    if ((n & 1ULL) == 0ULL)    /* n is even */
    {
        seq_t num = 4*sn*sn*sn + 35*sn - (seq_t)66;
        return num / (seq_t)6;
    }
    else                       /* n is odd */
    {
        seq_t num = 4*sn*sn*sn + 12*sn*sn + 35*sn - (seq_t)69;
        return num / (seq_t)6;
    }
}
