/*
 * A264041  a(n) is the maximum number of diagonals that can be placed in an n
 *          X n grid made up of 1 X 1 unit squares when diagonals are placed
 *          in the unit squares in such a way that no two diagonals may cross
 *          or intersect at an endpoint.
 * https://oeis.org/A264041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, nice
 * Formula:   for even n: a(n)=m*(2*m+1) where m=n/2; for odd n=2*m+1:
 *            a(n)=floor((6*m*m+10*m+4)/3)
 * Range:     u8 n <= 22, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        seq_t m = n / 2;
        return m * (2 * m + 1);
    } else {
        seq_t m = (n - 1) / 2;
        return (6 * m * m + 10 * m + 4) / 3;
    }
}
