/*
 * A275627  Expansion of (6*x^5+5*x^4+4*x^3+3*x^2+2*x+8)/(1-x-x^6).
 * https://oeis.org/A275627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-6) for n>5 a(0)=8, a(1)=10, a(2)=13, a(3)=17,
 *            a(4)=22, a(5)=28
 * Range:     u8 n <= 13, u16 n <= 35, u32 n <= 80, u64 n <= 168,
 *            u128 n <= 345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n = 0..5 */
    static const seq_t init[6] = {8,10,13,17,22,28};

    if (n < 6) {           /* small indices are trivial */
        return init[n];
    }

    /* buffer to hold the last six computed values */
    seq_t buf[6];
    for (int k = 0; k < 6; ++k) {
        buf[k] = init[k];
    }

    /* compute a(n) using the recurrence */
    for (idx_t i = 6; i <= n; ++i) {
        idx_t cur   = i % 6;                   /* position to store a(i) */
        seq_t val   = buf[(cur + 5) % 6] +      /* a(i-1)  */
                       buf[cur];                /* a(i-6)  */
        buf[cur] = val;
    }

    return buf[n % 6];
}
