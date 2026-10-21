/*
 * A279100  a(n) = Sum_{k=0..n} ceiling(phi^k), where phi is the golden ratio
 *          (A001622).
 * https://oeis.org/A279100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) + a(n-5) initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=11, a(4)=18, a(5)=30
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0..5 */
    const seq_t base[6] = {1, 3, 6, 11, 18, 30};

    if (n < 6)
        return base[n];

    /* Maintain the last five values: a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = base[0]; /* a(n-5) */
    seq_t a1 = base[1]; /* a(n-4) */
    seq_t a2 = base[2]; /* a(n-3) */
    seq_t a3 = base[3]; /* a(n-2) */
    seq_t a4 = base[4]; /* a(n-1) */
    seq_t a5 = base[5]; /* a(n)   for n=5 */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + a(i-2) - 3*a(i-3) + a(i-5) */
        seq_t next = ((seq_t)2) * a5 + a4;
        next -= ((seq_t)3) * a3;
        next += a1;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
