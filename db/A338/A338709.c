/*
 * A338709  Number of (undirected) paths in C_3 X P_n.
 * https://oeis.org/A338709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 73*a(n-2) + 180*a(n-3) - 219*a(n-4) +
 *            126*a(n-5) - 27*a(n-6) with initial values: a(1)=6; a(2)=129;
 *            a(3)=1209; a(4)=8856; a(5)=57522; a(6)=348945
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; a(0) is unused */
    static const seq_t init[6] = {6, 129, 1209, 8856, 57522, 348945};

    if (n <= 6) return init[n - 1];

    seq_t ring[6];
    for (idx_t i = 0; i < 6; ++i) ring[i] = init[i];

    seq_t val = 0;
    for (idx_t i = 7; i <= n; ++i)
    {
        /* compute positive part */
        seq_t pos = ring[5] * 14;
        pos += ring[3] * 180;
        pos += ring[1] * 126;

        /* compute negative part */
        seq_t neg = ring[4] * 73;
        neg += ring[2] * 219;
        neg += ring[0] * 27;

        val = pos - neg;              /* pos >= neg by construction */

        /* shift ring to keep last 6 values */
        for (idx_t j = 0; j < 5; ++j) ring[j] = ring[j + 1];
        ring[5] = val;
    }
    return val;
}
