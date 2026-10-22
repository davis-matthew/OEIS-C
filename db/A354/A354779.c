/*
 * A354779  a(0), ..., a(5) are 0, 1, 2, 3, 3, 3; thereafter if n == 0 mod 4
 *          then a(n) = n/2; if n == 1 mod 4, (n+1)/2; 2 mod 4, (n+2)/2; 3 mod
 *          4, (n+1)/2.
 * https://oeis.org/A354779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n>=6: a(n)= n%4==0 => n/2, n%4==1 => (n+1)/2, n%4==2 =>
 *            (n+2)/2, n%4==3 => (n+1)/2; initial terms
 *            a(0)=0,a(1)=1,a(2)=2,a(3)=3,a(4)=3,a(5)=3
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t first6[6] = {0, 1, 2, 3, 3, 3};
    if (n < 6) {
        return first6[n];
    }
    idx_t mod = n & 3U;                 /* n % 4 */
    seq_t result;
    switch (mod) {
        case 0:
            result = (seq_t)(n / 2);
            break;
        case 1:
        case 3:
            result = (seq_t)((n + 1) / 2);
            break;
        case 2:
            result = (seq_t)((n + 2) / 2);
            break;
    }
    return result;
}
