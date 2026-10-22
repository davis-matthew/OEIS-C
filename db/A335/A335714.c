/*
 * A335714  The sum of the sizes (positions) of fixed points over all
 *          compositions of n.
 * https://oeis.org/A335714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 4*a(n-3) + 3*a(n-4) + 2*a(n-5) for
 *            n ≥ 6 with the initial values a(1)=1, a(2)=1, a(3)=4, a(4)=8,
 *            a(5)=19
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n==0 for safety. */
    if (n == 0) return 0;

    /* Base terms (index == n).  base[0] is unused. */
    static const seq_t base[6] = {0, 1, 1, 4, 8, 19};

    if (n <= 5) return base[n];

    /* Shifted variables represent a(n-5),...,a(n-1). */
    seq_t a_minus5 = base[1];
    seq_t a_minus4 = base[2];
    seq_t a_minus3 = base[3];
    seq_t a_minus2 = base[4];
    seq_t a_minus1 = base[5];
    seq_t a_cur = 0;

    for (idx_t i = 6; i <= n; ++i)
    {
        a_cur = 4 * a_minus1
                - 3 * a_minus2
                - 4 * a_minus3
                + 3 * a_minus4
                + 2 * a_minus5;
        /* Advance the window for the next iteration. */
        a_minus5 = a_minus4;
        a_minus4 = a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = a_cur;
    }

    return a_minus1;   /* a_cur at the last iteration */
}
