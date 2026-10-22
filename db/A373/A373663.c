/*
 * A373663  a(n) = (1 + (n+2)^2 + (n-3)*(-1)^n)/2.
 * https://oeis.org/A373663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+2)^2 + (n-3)*(-1)^n)/2
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t t = n_seq + (seq_t)2;          /* n + 2 */
    seq_t t2 = t * t;                    /* (n + 2)^2 */

    seq_t num;
    if ((n & 1) == 0) {                  /* even n */
        num = t2 + n_seq - (seq_t)2;     /* (n+2)^2 + n - 2 */
    } else {                             /* odd n */
        num = t2 - n_seq + (seq_t)4;     /* (n+2)^2 - n + 4 */
    }
    return num / (seq_t)2;
}
