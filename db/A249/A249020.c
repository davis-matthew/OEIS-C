/*
 * A249020  a(n) = floor( n * (n+5) / 10) + 1.
 * https://oeis.org/A249020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n+5) / 10 ) + 1
 * Range:     u8 n <= 48, u16 n <= 807, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* split n = 10*u + t  ( 0 <= t < 10 ) to avoid overflow of n*(n+5)  */
    seq_t u = nn / 10;
    seq_t t = nn % 10;

    /* compute floor( n*(n+5) / 10 ) using the algebraic expansion:
       n = 10*u + t  =>  n*(n+5) = 100*u*u + 10*u*(2*t+5) + t*(t+5)
       dividing by 10 gives 10*u*u + u*(2*t+5) + floor( (t*t+5*t)/10 )   */
    seq_t part1 = 10 * u * u;                /* 10*u*u  */
    seq_t part2 = u * (2 * t + 5);           /* u*(2*t+5)  */
    seq_t part3 = (t * t + 5 * t) / 10;      /* floor( (t*t+5*t)/10 ) */

    return part1 + part2 + part3 + (seq_t)1;
}
