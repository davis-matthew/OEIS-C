/*
 * A309214  a(0)=0; thereafter a(n) = a(n-1)+n if a(n-1) even, otherwise a(n)
 *          = a(n-1)-n.
 * https://oeis.org/A309214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4t)=0, a(4t+1)=4t+1, a(4t+2)=-1, a(4t+3)=-(4t+4)
 * Range:     i8 n <= 128, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n & 3;            /* n % 4 */
    seq_t t = (seq_t)(n >> 2);  /* t = n / 4 */

    switch (r) {
        case 0: return 0;
        case 1: return t * 4 + 1;
        case 2: return (seq_t)-1;
        case 3: return -(t * 4 + 4);
        default: return 0; /* unreachable */
    }
}
