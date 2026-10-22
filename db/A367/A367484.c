/*
 * A367484  Number of integers of the form (x^4 + y^4) mod 3^n; a(n) =
 *          A289559(3^n).
 * https://oeis.org/A367484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-4) - 3*a(n-5) with initial values a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=19, a(4)=55
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle the initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)19;
    if (n == 4) return (seq_t)55;

    /* ring buffer for the last five values */
    seq_t buf[5] = { (seq_t)1, (seq_t)3, (seq_t)7, (seq_t)19, (seq_t)55 };

    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = 3 * buf[(i - 1) % 5] + buf[(i - 4) % 5]
                    - 3 * buf[(i - 5) % 5];
        buf[i % 5] = val;          /* store new value */
    }

    return buf[n % 5];              /* the value for index n */
}
