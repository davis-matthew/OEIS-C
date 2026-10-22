/*
 * A324245  The modified Collatz map considered by Vaillant and Delarue.
 * https://oeis.org/A324245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)/2 if n is odd, 3*n/4 if n % 4 == 0, (n-2)/4 if n
 *            % 4 == 2
 * Range:     u8 n <= 170, u16 n <= 9843, u32 n <= 9843, u64 n <= 9843,
 *            u128 n <= 9843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (sn & 1) {                    /* n odd */
        return (sn * 3 + 1) / 2;
    } else if ((sn & 3) == 0) {      /* n == 0 mod 4 */
        return sn * 3 / 4;
    } else {                         /* n == 2 mod 4 */
        return (sn - 2) / 4;
    }
}
