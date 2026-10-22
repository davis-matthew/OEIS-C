/*
 * A331052  Coordination sequence for rhombicosidodecahedron.
 * https://oeis.org/A331052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(8-n) for n >= 5; for 0 <= n <= 4 a(n) = 1 + 3*n +
 *            n*(n-1)/2 - 2*n*(n-1)*(n-2)/6 + n*(n-1)*(n-2)*(n-3)/24 (derived
 *            from the coordination sequence of the rhombicosidodecahedron)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 8)               /* beyond the diameter of the graph */
        return 0;

    /* use the symmetry a(n)=a(8-n) for n>4 */
    idx_t m = n;
    if (m > 4)
        m = 8 - m;

    seq_t n1 = (seq_t)m;
    seq_t n2 = 0;
    seq_t n3 = 0;
    seq_t n4 = 0;

    if (m >= 1)
        n2 = (seq_t)m * ((seq_t)m - 1);
    if (m >= 2)
        n3 = (seq_t)m * ((seq_t)m - 1) * ((seq_t)m - 2);
    if (m >= 3)
        n4 = (seq_t)m * ((seq_t)m - 1) * ((seq_t)m - 2) * ((seq_t)m - 3);

    seq_t res = 1;
    res += 3 * n1;                 /* 3*n */
    res += n2 / 2;                 /* + n*(n-1)/2 */
    seq_t t3 = n3 / 6;              /* n*(n-1)*(n-2)/6 */
    res -= 2 * t3;                  /* - 2 * that term */
    seq_t t4 = n4 / 24;             /* n*(n-1)*(n-2)*(n-3)/24 */
    res += t4;                      /* + that term */

    return res;
}
