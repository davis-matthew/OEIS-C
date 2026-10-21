/*
 * A296910  a(0)=1, a(1)=4; thereafter a(n) = 4*n-2*(-1)^n.
 * https://oeis.org/A296910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 2*(-1)^n (a(0) = 1, a(1) = 4)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (n == 0) ? (seq_t)1 : (seq_t)4;
    }
    if ((n & 1) == 0) {                /* n is even */
        return (seq_t)n * (seq_t)4 - (seq_t)2;
    } else {                            /* n is odd */
        return (seq_t)n * (seq_t)4 + (seq_t)2;
    }
}
