/*
 * A319702  Filter sequence for sequences that are constant for all even terms
 *          >= 2.
 * https://oeis.org/A319702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n = 1, a(1) = 1; * for n > 1, if n is even, a(n) = 2, *
 *            otherwise a(n) = (n + 3) / 2
 * Range:     u8 n <= 488, u16 n <= 8620, u32 n <= 8620, u64 n <= 8620,
 *            u128 n <= 8620, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;
    }
    /* Even index? */
    if ((n & 1) == 0) {
        return 2;
    }
    /* Odd index: (n + 3) / 2 */
    return (seq_t)(n + 3) / 2;
}
