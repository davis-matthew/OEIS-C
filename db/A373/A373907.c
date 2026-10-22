/*
 * A373907  Number of compositions of 7*n into parts 1 and 7.
 * https://oeis.org/A373907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            8*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)+a(n-7)
 *            Initial values: a(0)=1, a(1)=2, a(2)=10, a(3)=53, a(4)=264,
 *            a(5)=1294, a(6)=6349
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A373907 */
seq_t a(idx_t n)
{
    /* Initial values for n = 0..6 */
    static const seq_t init[7] = {
        (seq_t)1, (seq_t)2, (seq_t)10,
        (seq_t)53, (seq_t)264, (seq_t)1294,
        (seq_t)6349
    };

    if (n < 7)
        return init[n];

    /* Circular buffer of the last seven values */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    /* Compute sequentially up to n */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t r = (seq_t)8   * buf[(i - 1) % 7];
        r = r - (seq_t)21 * buf[(i - 2) % 7];
        r = r + (seq_t)35 * buf[(i - 3) % 7];
        r = r - (seq_t)35 * buf[(i - 4) % 7];
        r = r + (seq_t)21 * buf[(i - 5) % 7];
        r = r - (seq_t)7  * buf[(i - 6) % 7];
        r = r +         buf[(i - 7) % 7];
        buf[i % 7] = r;
    }

    return buf[n % 7];
}
