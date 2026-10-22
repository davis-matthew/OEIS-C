/*
 * A308364  a(0) = 0, a(3n) = a(n), a(3n+1) = a(n)*3 + 1, a(3n-1) = a(n)*3 -
 *          1.
 * https://oeis.org/A308364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(0)=0; a(3n)=a(n); a(3n+1)=a(n)*3+1; a(3n-1)=a(n)*3-1
 * Range:     u8 n <= 283, u16 n <= 9573, u32 n <= 9573, u64 n <= 9573,
 *            u128 n <= 9573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t q = n / 3;
    int r = n % 3;
    if (r == 0) {
        return a(q);
    } else if (r == 1) {
        return a(q) * 3 + 1;
    } else { /* r == 2 : n == 3*q + 2 == 3*(q+1) - 1 */
        return a(q + 1) * 3 - 1;
    }
}
