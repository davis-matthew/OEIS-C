/*
 * A354529  a(1) = 3, a(2) = 12 and a(n) = (3n^2+8n-2)/2 if n is even or =
 *          (3n^2+8n-5)/2, if n is odd, for n >= 3.
 * https://oeis.org/A354529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=12, a(n) = (3n^2+8n-2)/2 if n even, a(n) =
 *            (3n^2+8n-5)/2 if n odd (for n >= 3)
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)3;
    }
    if (n == (idx_t)2) {
        return (seq_t)12;
    }

    seq_t nn = (seq_t)n;            /* promote index to seq_t */
    seq_t val = nn * nn;            /* n^2  */
    val = val * (seq_t)3;           /* 3n^2 */
    val = val + nn * (seq_t)8;      /* +8n */

    /* subtract 2 for even n, 5 for odd n */
    if ((n & (idx_t)1) == (idx_t)0) {    /* even */
        val = val - (seq_t)2;
    } else {                            /* odd */
        val = val - (seq_t)5;
    }

    return val / (seq_t)2;                /* integer division */
}
