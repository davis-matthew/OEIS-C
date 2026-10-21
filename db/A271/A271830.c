/*
 * A271830  Expansion of (3 - 4*x + 3*x^2 + x^4)/((1 - x)^2*(1 + x^2 + x^4)).
 * https://oeis.org/A271830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pattern: a(n) = n+3 if n%6==0; a(n) = n-1 if n%6==2; otherwise a(n)=n+1 */
/* a(0)=3, a(1)=2, a(2)=1, a(3)=4, a(4)=5, a(5)=6 */
seq_t a(idx_t n)
{
    idx_t r = n % 6;
    if (r == 0)
        return (seq_t)(n + 3);
    else if (r == 2)
        return (seq_t)(n - 1);
    else
        return (seq_t)(n + 1);
}
