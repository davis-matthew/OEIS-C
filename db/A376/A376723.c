/*
 * A376723  Expansion of 1/((1 - x^2 - x^3)^2 - 4*x^5).
 * https://oeis.org/A376723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3) - a(n-4) + 2*a(n-5) - a(n-6) with
 *            a(0)=1, a(1)=0, a(2)=2, a(3)=2, a(4)=3, a(5)=10, a(6)=7
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Predefined starting values (fits in seq_t) */
    seq_t a[7];
    a[0] = 1;  /* a(0) */
    a[1] = 0;  /* a(1) */
    a[2] = 2;  /* a(2) */
    a[3] = 2;  /* a(3) */
    a[4] = 3;  /* a(4) */
    a[5] = 10; /* a(5) */
    a[6] = 7;  /* a(6) */

    if (n < 7)
        return a[n];

    for (idx_t i = 7; i <= n; ++i)
    {
        /* indices modulo 7 */
        idx_t i2 = (i - 2) % 7;
        idx_t i3 = (i - 3) % 7;
        idx_t i4 = (i - 4) % 7;
        idx_t i5 = (i - 5) % 7;
        idx_t i6 = (i - 6) % 7;

        seq_t next = 2 * a[i2] + 2 * a[i3] - a[i4] + 2 * a[i5] - a[i6];
        a[i % 7] = next;
    }

    return a[n % 7];
}
