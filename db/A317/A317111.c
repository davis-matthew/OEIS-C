/*
 * A317111  Number of permutations of [n] in which the length of every
 *          increasing run is 0 or 1 (mod 4).
 * https://oeis.org/A317111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * a(n‑1) - [n*(n-1)/2] * a(n‑2) + [n*(n-1)*(n-2)/6] *
 *            a(n‑3) with a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

    /* a0 = a(n‑3), a1 = a(n‑2), a2 = a(n‑1) */
    seq_t a0 = (seq_t)1, a1 = (seq_t)1, a2 = (seq_t)1, ai = a2;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t ni = (seq_t)i;

        /* t1 = ni * a2 */
        seq_t t1 = ni * a2;

        /* t2 = ni * (ni-1) / 2 * a1 */
        seq_t t2 = ni * (ni - 1);
        t2 = t2 / 2;
        t2 = t2 * a1;

        /* t3 = ni * (ni-1) * (ni-2) / 6 * a0 */
        seq_t t3 = ni * (ni - 1);
        t3 = t3 * (ni - 2);
        t3 = t3 / 6;
        t3 = t3 * a0;

        ai = t1 - t2 + t3;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return ai;
}
