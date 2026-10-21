/*
 * A294806  Number of permutations of [n] avoiding {1324, 3421, 3241}.
 * https://oeis.org/A294806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*(24*(2^(2 + n)-3) + 5*(2^n-16)*n - 6*(2^n+2)*n^2 +
 *            (2^n-4)*n^3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 51, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=12*a(n-1)-62*a(n-2)+180*a(n-3)-321*a(n-4)+360*a(n-5)-248*a(n-6)+96*a(n-7)-16*a(n-8), n>7 */
/* Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75, a(6)=259, a(7)=852 */

seq_t a(idx_t n)
{
    /* pre‑compute the first eight terms once */
    static seq_t base[8];
    static int base_inited = 0;
    if (!base_inited) {
        const unsigned long long tmp[8] = {1ULL,1ULL,2ULL,6ULL,21ULL,75ULL,259ULL,852ULL};
        for (idx_t i = 0; i < 8; ++i)
            base[i] = (seq_t)tmp[i];
        base_inited = 1;
    }

    if (n < 8)
        return base[n];

    /* ring buffer holding the last eight values */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t val;
        val = buf[7] * (seq_t)12;     /* +12*a(n-1)   */
        val -= buf[6] * (seq_t)62;     /* -62*a(n-2)   */
        val += buf[5] * (seq_t)180;    /* +180*a(n-3)  */
        val -= buf[4] * (seq_t)321;    /* -321*a(n-4)  */
        val += buf[3] * (seq_t)360;    /* +360*a(n-5)  */
        val -= buf[2] * (seq_t)248;    /* -248*a(n-6)  */
        val += buf[1] * (seq_t)96;     /* +96*a(n-7)   */
        val -= buf[0] * (seq_t)16;     /* -16*a(n-8)   */

        /* shift the buffer: drop a(n-8), shift left, insert new a(n) */
        for (idx_t j = 0; j < 7; ++j)
            buf[j] = buf[j + 1];
        buf[7] = val;
    }

    return buf[7];
}
