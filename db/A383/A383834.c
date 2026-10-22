/*
 * A383834  Sum of the legs of the unique primitive Pythagorean triple whose
 *          inradius is A000217(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A383834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ((n*(n+1)/2)^2) + 4 * (n*(n+1)/2) + 1
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = N * (N + 1) / 2;     /* triangular number T(n) */
    return 2 * t * t + 4 * t + 1;  /* a(n) = 2*T^2 + 4*T + 1 */
}
