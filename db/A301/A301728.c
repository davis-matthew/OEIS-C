/*
 * A301728  a(0)=1; thereafter, a(n) = 2n-1 if n == 0 (mod 3), (5n+1)/3 if n
 *          == 1 (mod 3), (5n+2)/3 if n == 2 (mod 3).
 * https://oeis.org/A301728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=2*n-1 if n%3==0, (5*n+1)/3 if n%3==1, (5*n+2)/3 if
 *            n%3==2
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t r = n % 3;
    seq_t fn = (seq_t)n;

    if (r == 0)
        return 2 * fn - 1;
    else if (r == 1)
        return (5 * fn + 1) / 3;
    else
        return (5 * fn + 2) / 3;
}
