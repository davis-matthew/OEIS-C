/*
 * A271054  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 259", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 5 a(n) = 5 for even n >= 2 a(2k+1) = 16*(k-1)^2
 *            + 56*(k-1) + 36 for odd n = 2k+1 >= 3 Here k = n/2 (integer
 *            division). Phase diagram of Rule 259 yields these active cell
 *            counts
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    if ((n & 1) == 0)               /* even n >= 2 */
        return 5;

    /* odd n >= 3 */
    idx_t k   = n >> 1;            /* k = (n-1)/2 */
    idx_t k0  = k - 1;             /* k0 = (n-3)/2 */
    seq_t  res = 16 * (seq_t)k0 * (seq_t)k0;
    res += 56 * (seq_t)k0;
    res += 36;
    return res;
}
