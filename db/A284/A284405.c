/*
 * A284405  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 913", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=5, a(3)=7; for n>=4, a(n)=2*a(n-1)+1
 *            (equivalently a(n)=2^(n+1)-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)7;

    /* For n >= 4, use the recurrence a(n) = 2*a(n-1) + 1, starting
       from a(4) = 31. This works for all supported seq_t sizes,
       including arbitrary‑precision, and avoids undefined behaviour
       from shifting by too many bits. */
    seq_t val = (seq_t)31;          /* a(4) */
    idx_t i;

    for (i = 5; i <= n; ++i) {
        val = (val << 1) + (seq_t)1;  /* val = 2*val + 1 */
    }
    return val;
}
