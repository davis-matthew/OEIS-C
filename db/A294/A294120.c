/*
 * A294120  E.g.f.: exp(3*((1+x)^3 - 1)).
 * https://oeis.org/A294120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 18*(n-1)*a(n-2) + 9*(n-1)*(n-2)*a(n-3) a(0) =
 *            1, a(1) = 9, a(2) = 99
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)9;
    if (n == 2u) return (seq_t)99;

    /* Iterate using the recurrence; keep only the last three values */
    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)9;          /* a(1) */
    seq_t a2 = (seq_t)99;         /* a(2) */
    seq_t a3;                     /* a(i) to be computed */

    for (idx_t i = 3u; i <= n; ++i) {
        /* n-1 and n-2 used in the recurrence */
        seq_t n1 = (seq_t)(i - 1u);
        seq_t n2 = (seq_t)(i - 2u);

        /* Apply the recurrence relation */
        seq_t term1 = (seq_t)9 * a2;
        seq_t term2 = (seq_t)18 * n1 * a1;
        seq_t term3 = (seq_t)9 * n1 * n2 * a0;

        a3 = term1 + term2 + term3;

        /* Shift the sliding window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
