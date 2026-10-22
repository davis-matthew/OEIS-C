/*
 * A329774  a(n) = n+1 for n <= 2; otherwise a(n) = 3*a(n-3)+1.
 * https://oeis.org/A329774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3; for n>2, a(n)=3*a(n-3)+1
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 59, u64 n <= 120,
 *            u128 n <= 240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values directly. */
    if (n <= 2)
        return (seq_t)n + (seq_t)1;

    /* Use a cycling buffer of the last three computed terms. */
    seq_t vals[3];
    vals[0] = (seq_t)1;          /* a(0) */
    vals[1] = (seq_t)2;          /* a(1) */
    vals[2] = (seq_t)3;          /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        vals[i % 3] = (seq_t)3 * vals[(i - 3) % 3] + (seq_t)1;
    }

    return vals[n % 3];
}
