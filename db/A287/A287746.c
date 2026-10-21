/*
 * A287746  First differences of A154293.
 * https://oeis.org/A287746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/2 if n%4 == 0 * a(n) = 5*n/2 if n%4 == 2 * a(n) =
 *            2*floor(3*n/4)+1 if n is odd
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res;
    if ((n & 1) == 0) {
        if ((n & 2) == 0) {
            /* n divisible by 4 */
            res = (seq_t)n / 2;
        } else {
            /* n ≡ 2 (mod 4) */
            res = (seq_t)5 * (seq_t)n / 2;
        }
    } else {
        /* n odd */
        seq_t t = (seq_t)3 * (seq_t)n;
        t /= 4;              /* floor division */
        res = (seq_t)2 * t + 1;
    }
    return res;
}
