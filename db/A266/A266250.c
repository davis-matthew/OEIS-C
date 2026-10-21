/*
 * A266250  Total number of ON (black) cells after n iterations of the "Rule
 *          9" elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2-(-1)^n*(n-4)+2)/2 for n > 2 a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* For n ≥ 3 use the closed form.  The expression
       can be split according to the parity of n, which
       keeps all intermediate values non‑negative. */
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {           /* n even */
        /* a(n) = (n^2 - n + 6) / 2 */
        return ((nn * nn) - nn + (seq_t)6) / ((seq_t)2);
    } else {                      /* n odd */
        /* a(n) = (n^2 + n - 2) / 2 */
        return ((nn * nn) + nn - (seq_t)2) / ((seq_t)2);
    }
}
