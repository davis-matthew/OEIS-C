/*
 * A319384  a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5), a(0)=1,
 *          a(1)=5, a(2)=9, a(3)=21, a(4)=29.
 * https://oeis.org/A319384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2 + 6*n + 5 - (2*n + 1)*(-1)^n)/4
 * Range:     u8 n <= 6, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                 /* convert index to seq_t type */
    seq_t term1 = (seq_t)6 * N * N + (seq_t)6 * N + 5;
    seq_t twoNplus1 = N * (seq_t)2 + 1;
    seq_t numerator;

    if ((n & 1) == 0)   /* n even: (-1)^n = 1  -> subtract (2n+1) */
        numerator = term1 - twoNplus1;
    else                 /* n odd:  (-1)^n = -1 -> add (2n+1) */
        numerator = term1 + twoNplus1;

    return numerator / 4;
}
