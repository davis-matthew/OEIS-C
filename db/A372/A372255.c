/*
 * A372255  a(1) = 1, a(n) = n*(n-2)! + n - 1.
 * https://oeis.org/A372255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-2)! + n - 1 (n>=2, a(1)=1) Recurrence (valid for
 *            n>=5): a(n) = (n-1)*a(n-1) - (n-4)*a(n-3) - (4*n-13) Initial
 *            values: a(1)=1, a(2)=3, a(3)=5, a(4)=11
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small indices handled directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)11;

    /* Use a circular buffer of size 5 to hold the last 4 sequence values.
       Indices used: 0..4, where buf[k] holds a[k] for k = 1..4 initially. */
    seq_t buf[5];
    buf[1] = (seq_t)1;   /* a(1) */
    buf[2] = (seq_t)3;   /* a(2) */
    buf[3] = (seq_t)5;   /* a(3) */
    buf[4] = (seq_t)11;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t term1 = (seq_t)(i - 1) * buf[(i - 1) % 5];
        seq_t term2 = (seq_t)(i - 4) * buf[(i - 3) % 5];
        seq_t term3 = (seq_t)(4 * i - 13);
        seq_t ai     = term1 - term2 - term3;
        buf[i % 5]   = ai;
    }

    return buf[n % 5];
}
