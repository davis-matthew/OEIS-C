/*
 * A291002  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S)*(1 - 2*S)*(1 -
 *          3*S).
 * https://oeis.org/A291002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 26*a(n-2) + 24*a(n-3) for n ≥ 4 Initial
 *            values: a(0) = 6, a(1) = 31, a(2) = 146, a(3) = 652
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)6ULL;
    if (n == 1) return (seq_t)31ULL;
    if (n == 2) return (seq_t)146ULL;
    if (n == 3) return (seq_t)652ULL;

    /* Initialise the last three computed terms. */
    seq_t a1 = (seq_t)31ULL;   /* a(n-3) when n==4 */
    seq_t a2 = (seq_t)146ULL;  /* a(n-2) when n==4 */
    seq_t a3 = (seq_t)652ULL;  /* a(n-1) when n==4 */
    seq_t a_next;

    /* Iterate until the requested index is reached. */
    for (idx_t i = 4; i <= n; ++i) {
        a_next = a3 * (seq_t)9;
        a_next -= a2 * (seq_t)26;
        a_next += a1 * (seq_t)24;

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }

    return a3;
}
