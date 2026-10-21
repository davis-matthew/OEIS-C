/*
 * A267047  Total number of ON (black) cells after n iterations of the "Rule
 *          91" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=3; for n≥2, if n even: a(n) = (n^2 + 3n)/2 if n odd
 *            : a(n) = (n^2 + 5n - 6)/2
 * Range:     u8 n <= 20, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {          /* n is even */
        return (sn * sn + 3 * sn) / 2;
    } else {                     /* n is odd  */
        return (sn * sn + 5 * sn - 6) / 2;
    }
}
