/*
 * A254007  Cardinality of the set of equivalence classes of the set X_n of
 *          finite integer sequences {x_1 = 0, x_2, ..., x_n} satisfying |x_k
 *          - x_{k+1}| = 1, where two such sequences are deemed equivalent if
 *          they are permutations of each other.
 * https://oeis.org/A254007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   for n ≥ 8, a(n) = a(n–1) + 5 a(n–2) – 4 a(n–3) – 8 a(n–4) + 4
 *            a(n–5) + 4 a(n–6). Initial values: a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=4, a(4)=7, a(5)=13. For n = 6 and 7 the same recurrence
 *            holds with extra constants: a(6) =
 *            a(5)+5a(4)–4a(3)–8a(2)+4a(1)+4a(0)–2, a(7) =
 *            a(6)+5a(5)–4a(4)–8a(3)+4a(2)+4a(1)+1
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values a(0)…a(5) */
    const seq_t init[6] = { 1, 1, 2, 4, 7, 13 };

    if (n < 6)
        return init[n];

    /* buffer holding the last six terms:
       buf[0] = a(n–6), buf[1] = a(n–5), … , buf[5] = a(n–1)   */
    seq_t buf[6] = { 1, 1, 2, 4, 7, 13 };
    seq_t res;

    for (idx_t i = 6; i <= n; ++i)
    {
        if (i == 6) {
            res = buf[5] + (seq_t)5 * buf[4];
            res -= (seq_t)4 * buf[3];
            res -= (seq_t)8 * buf[2];
            res += (seq_t)4 * buf[1];
            res += (seq_t)4 * buf[0];
            res -= (seq_t)2;                /* constant for n == 6   */
        } else if (i == 7) {
            res = buf[5] + (seq_t)5 * buf[4];
            res -= (seq_t)4 * buf[3];
            res -= (seq_t)8 * buf[2];
            res += (seq_t)4 * buf[1];
            res += (seq_t)4 * buf[0];
            res += (seq_t)1;                /* constant for n == 7   */
        } else {                            /* n ≥ 8, homogeneous part   */
            res = buf[5] + (seq_t)5 * buf[4];
            res -= (seq_t)4 * buf[3];
            res -= (seq_t)8 * buf[2];
            res += (seq_t)4 * buf[1];
            res += (seq_t)4 * buf[0];
        }

        /* shift buffer: drop oldest, append new term */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = res;
    }

    return buf[5];
}
