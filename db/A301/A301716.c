/*
 * A301716  Coordination sequence for node of type V1 in "kre" 2-D tiling (or
 *          net).
 * https://oeis.org/A301716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (x^8+6*x^7+12*x^6+17*x^5+12*x^4+17*x^3+12*x^2+6*x+1) /*
 *            /((1-x^3)*(1-x^5)) /* Recurrence: a(n) = a(n-3) + a(n-5) -
 *            a(n-8) for n >= 9 /* Initial values: a(0..8) =
 *            1,6,12,18,18,30,36,36,48
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0..8 */
    static const seq_t base[9] = {
        (seq_t)1, (seq_t)6, (seq_t)12, (seq_t)18, (seq_t)18,
        (seq_t)30, (seq_t)36, (seq_t)36, (seq_t)48
    };
    if (n < 9) return base[n];

    /* a circular buffer that always contains a[i-8] .. a[i] for current i */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i) buf[i] = base[i];

    for (idx_t i = 9; i <= n; ++i) {
        /* indices modulo 9 give the needed elements:
           a(i-3), a(i-5), a(i-8) are at positions (i-3)%9, (i-5)%9, (i-8)%9 */
        seq_t val = buf[(i - 3) % 9] + buf[(i - 5) % 9] - buf[(i - 8) % 9];
        buf[i % 9] = val;   /* store a(i) in the buffer */
    }

    return buf[n % 9];
}
