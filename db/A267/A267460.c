/*
 * A267460  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          133" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=2, a(n)=a(n-1)+a(n-2)-a(n-3) * but for n>1 the
 *            closed form is: * a(n) = (2*n-(-1)^n+5)/2 * equivalently: * a(n)
 *            = n+2 for even n * a(n) = n+3 for odd n
 * Range:     u8 n <= 252, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    /* For n > 1, a(n) = n + 2 if n is even, else n + 3 */
    if ((n & 1) == 0)
        return (seq_t)n + (seq_t)2;
    else
        return (seq_t)n + (seq_t)3;
}
