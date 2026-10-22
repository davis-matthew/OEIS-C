/*
 * A400485  a(1) = 1; for n > 1: if a(n-1) + n is odd, a(n) = a(n-1) + n else
 *          a(n) = (a(n-1) + n)/2.
 * https://oeis.org/A400485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(1) = 1; for n>1: if (a(n-1)+n) is odd, a(n) = a(n-1)+n; else
 *            a(n) = (a(n-1)+n)/2
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t prev = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = prev + (seq_t)i;
        if (sum & (seq_t)1) {          /* odd */
            prev = sum;
        } else {                       /* even */
            prev = sum >> 1;           /* divide by 2 */
        }
    }
    return prev;
}
