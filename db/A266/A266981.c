/*
 * A266981  Number of ON (black) cells in the n-th iteration of the "Rule 79"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 + (-1)^n - 2*(-2 + (-1)^n)*n)/4, equivalent to a(2*k)
 *            = k + 1 and a(2*k+1) = 3*k + 2
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1ULL) {                 /* n is odd : n = 2*k+1 */
        seq_t k = (seq_t)(n - 1) / 2;
        return 3 * k + 2;
    } else {                        /* n is even : n = 2*k */
        seq_t k = (seq_t)n / 2;
        return k + 1;
    }
}
