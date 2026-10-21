/*
 * A253434  Number of (n+1) X (7+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 588 for n > 1, a(1) = 613
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)613;
    /* For n > 1 use the closed‑form expression.
       The shift is performed on a seq_t value so that it works
       with arbitrary‑precision types. */
    return (seq_t)9 * (((seq_t)1) << (n - 1)) + (seq_t)588;
}
