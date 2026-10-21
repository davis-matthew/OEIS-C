/*
 * A271051  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 253", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=8; a(2)=4; a(n) = 13 for n>2 even; a(n) = 4*(n^2 +
 *            n - 1) for n>2 odd; (The recurrence
 *            a(n)=3*a(n-2)-3*a(n-4)+a(n-6) for n>8 is equivalent.)
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return 1;
    if (n == 1) return 8;
    if (n == 2) return 4;

    /* general rule */
    if ((n & 1) == 0)               /* even */
        return 13;

    /* odd: use 4*(n^2 + n - 1) */
    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2 */
    nsq += (seq_t)n;                   /* + n */
    nsq -= 1;                          /* - 1 */
    return 4 * nsq;
}
