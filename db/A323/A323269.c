/*
 * A323269  The sequence denoted by h_n used in the calculation of A323260.
 * https://oeis.org/A323269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 5, a(3) = 19, a(4) = 76, a(5) = 307, a(6) =
 *            1245 For n ≥ 7: a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) + 7*a(n-4)
 *            - a(n-5) This recurrence is derived from the generating function
 *            of the sequence. The first six terms are used as the initial
 *            conditions
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* values below the offset are irrelevant */

    static const seq_t base[6] = { 1, 5, 19, 76, 307, 1245 };

    if (n <= 6)
        return base[n - 1];

    /* start with a(2)..a(6) to compute a(7) */
    seq_t a2 = base[1];   /* a(2) */
    seq_t a3 = base[2];   /* a(3) */
    seq_t a4 = base[3];   /* a(4) */
    seq_t a5 = base[4];   /* a(5) */
    seq_t a6 = base[5];   /* a(6) */
    seq_t cur;            /* a(n) being computed */

    for (idx_t i = 7; i <= n; ++i)
    {
        /* compute a(i) using the 5‑term recurrence */
        cur = (seq_t)5 * a6 + (seq_t)7 * a3
              - ((seq_t)3 * a5 + (seq_t)5 * a4 + a2);

        /* shift the window for the next iteration */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = cur;
    }

    return cur;
}
