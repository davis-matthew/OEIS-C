/*
 * A385643  Galileo sequence with ratio k = 5: a(1) = 1, a(2) = k, a(2*n-1) =
 *          floor(((k + 1)*a(n) -1)/2), and a(2*n) = floor((k + 1)*a(n)/2) + 1
 *          for n > 2.
 * https://oeis.org/A385643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(1)=1, a(2)=5, k=5. For n>2: a(2*n-1) = floor(((k+1)*a(n)-1)/2)
 *            a(2*n) = floor((k+1)*a(n)/2)+1
 * Range:     u8 n <= 16, u16 n <= 512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* Compute from the smaller index */
    const seq_t mult = (seq_t)6;            /* k + 1 = 6 */
    if (n & 1U) {                          /* n is odd: n = 2*m-1 */
        idx_t m = (n + 1) / 2;              /* m = (n+1)/2 */
        seq_t a_m = a(m);
        return (mult * a_m - (seq_t)1) / (seq_t)2;
    } else {                               /* n is even: n = 2*m */
        idx_t m = n / 2;
        seq_t a_m = a(m);
        return (mult * a_m) / (seq_t)2 + (seq_t)1;
    }
}
