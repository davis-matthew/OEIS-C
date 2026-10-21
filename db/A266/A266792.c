/*
 * A266792  Number of ON (black) cells in the n-th iteration of the "Rule 61"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 for even n > 4, a(n) = 2*n - 3 for odd n > 4, with
 *            initial values a(0)=1, a(1)=2, a(2)=2, a(3)=5, a(4)=4
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n = 0..4 */
    static const seq_t init[5] = {1, 2, 2, 5, 4};

    if (n <= 4)
        return init[n];

    /* even indices (n > 4) */
    if ((n & 1u) == 0)
        return (seq_t)5;

    /* odd indices (n > 4) */
    return (seq_t)((seq_t)2 * (seq_t)n - (seq_t)3);
}
