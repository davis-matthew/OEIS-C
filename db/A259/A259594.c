/*
 * A259594  Denominators of the other-side convergents to sqrt(6).
 * https://oeis.org/A259594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 10*a(n-2) - a(n-4) with initial values: a(0)=1, a(1)=3,
 *            a(2)=11, a(3)=29
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)29;

    /* Use a circular buffer of size 4 to keep the last four terms.
       Positions correspond to a(i%4). */
    seq_t buf[4] = { (seq_t)1,   /* a(0) */ 
                     (seq_t)3,   /* a(1) */ 
                     (seq_t)11,  /* a(2) */ 
                     (seq_t)29 }; /* a(3) */

    /* Compute iteratively from i = 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = (buf[(i - 2) % 4] * (seq_t)10) - buf[(i - 4) % 4];
        buf[i % 4] = val;
    }

    return buf[n % 4];
}
