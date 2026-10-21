/*
 * A275635  a(n) = (3^n-1)*(3^n-3)*(3^n+3)/4!.
 * https://oeis.org/A275635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n-1)*(3^n-3)*(3^n+3)/4! Recurrence (for n>3):
 *            a(n)=40*a(n-1)-390*a(n-2)+1080*a(n-3)-729*a(n-4) Initial terms:
 *            a(0)=0, a(1)=0, a(2)=24, a(3)=780
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* starting values for a(0) … a(3) */
    seq_t v[4] = {(seq_t)0, (seq_t)0, (seq_t)24, (seq_t)780};

    if (n <= 3)
        return v[n];

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = 40 * v[3];
        an = an - 390 * v[2];
        an = an + 1080 * v[1];
        an = an - 729 * v[0];

        /* shift the window: v[0]←v[1], v[1]←v[2], v[2]←v[3], v[3]←an */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = an;
    }

    return v[3];
}
