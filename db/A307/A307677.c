/*
 * A307677  a(0) = a(1) = a(2) = a(3) = 1; thereafter a(n) = a(n-1) + a(n-2) +
 *          a(n-4).
 * https://oeis.org/A307677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) for n>3, a(0) = a(1) = a(2) =
 *            a(3) = 1
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 41, u64 n <= 81, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial four terms */
    if (n <= 3)
        return (seq_t)1;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)1, a1 = (seq_t)1, a2 = (seq_t)1, a3 = (seq_t)1;
    seq_t an = a0; /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 + a2 + a0; /* a(n) = a(n-1) + a(n-2) + a(n-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
