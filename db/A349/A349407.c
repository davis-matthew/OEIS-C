/*
 * A349407  The Farkas map: a(n) = x/3 if x mod 3 = 0; a(n) = (3x+1)/2 if x
 *          mod 3 <> 0 and x mod 4 = 3; a(n) = (x+1)/2 if x mod 3 <> 0 and x
 *          mod 4 = 1, where x = 2*n-1.
 * https://oeis.org/A349407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)/3 if (2*n-1)%3==0; a(n) = (3*(2*n-1)+1)/2 if
 *            (2*n-1)%4==3; a(n) = ((2*n-1)+1)/2 otherwise
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = ((seq_t)n << 1) - (seq_t)1;  /* x = 2*n-1 */
    if (x % 3 == 0)
        return x / 3;
    else if (x % 4 == 3)
        return (3 * x + 1) / 2;
    else
        return (x + 1) / 2;
}
