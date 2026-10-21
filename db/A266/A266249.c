/*
 * A266249  Number of ON (black) cells in the n-th iteration of the "Rule 9"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=2, a(3)=2; for n>3: if n is even → a(n)=4;
 *            if n is odd → a(n)=2*n-5
 * Range:     u8 n <= 130, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first few values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)2;

    /* for n >= 4 */
    if ((n & 1) == 0)           /* even n */
        return (seq_t)4;
    else {                     /* odd n */
        seq_t nn = (seq_t)n;
        return (seq_t)(2 * nn - (seq_t)5);
    }
}
