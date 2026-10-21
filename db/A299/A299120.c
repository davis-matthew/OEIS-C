/*
 * A299120  a(n) = (n-1)*(n-2)*(n+3)*(n+2)/12.
 * https://oeis.org/A299120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-2)*(n+3)*(n+2)/12
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 273, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        /* a(0)=1, a(1)=0, a(2)=0 */
        return (n == 0) ? (seq_t)1 : (seq_t)0;
    }
    seq_t t1 = (seq_t)(n - 1);
    seq_t t2 = (seq_t)(n - 2);
    seq_t t3 = (seq_t)(n + 2);
    seq_t t4 = (seq_t)(n + 3);
    return (t1 * t2 * t3 * t4) / (seq_t)12;
}
