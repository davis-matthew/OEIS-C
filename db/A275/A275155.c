/*
 * A275155  a(1) = 18; a(n) = 3*a(n - 1) + 2*sqrt(2*a(n - 1)*(a(n - 1) - 14))
 *          - 14 for n > 1.
 * https://oeis.org/A275155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) for n > 3 * Initial values:
 *            a(1) = 18, a(2) = 64, a(3) = 338
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1 – values for n < 1 are irrelevant */
    if (n == 0) return (seq_t)0;

    /* handle the first three terms explicitly */
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)64;
    if (n == 3) return (seq_t)338;

    /* use the linear recurrence to generate terms up to n */
    seq_t a1 = (seq_t)18;      /* a(n-3) */
    seq_t a2 = (seq_t)64;      /* a(n-2) */
    seq_t a3 = (seq_t)338;     /* a(n-1) */
    seq_t seven = (seq_t)7;

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = seven * a3 - seven * a2 + a1;  /* a(i) */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
