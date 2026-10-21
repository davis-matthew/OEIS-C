/*
 * A262672  Expansion of (3-x-x^3) / ((x-1)^2*(1+x+x^2+x^3)).
 * https://oeis.org/A262672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/4)+3 for n%4==0 * a(n) = (n/4)+2 for n%4==1 or 2 *
 *            a(n) = (n/4)+1 for n%4==3 * This satisfies
 *            a(n)=a(n-1)+a(n-4)-a(n-5) for n>4 with a(0)=3, a(1)=2, a(2)=2,
 *            a(3)=1, a(4)=4
 * Range:     u8 n <= 400, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute quotient and remainder of n by 4. */
    idx_t k = n / 4;
    idx_t r = n % 4;

    /* Return the appropriate arithmetic expression. */
    switch (r) {
    case 0:
        return (seq_t)(k + 3);
    case 1:
    case 2:
        return (seq_t)(k + 2);
    case 3:
        return (seq_t)(k + 1);
    default:
        /* Should never happen */
        return (seq_t)0;
    }
}
