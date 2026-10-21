/*
 * A291257  a(n) = (1/2)*A291228(n).
 * https://oeis.org/A291257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3) - a(n-4) for n >= 5
 *            Initial values: a(0) = 1 a(1) = 3 a(2) = 9 a(3) = 28 a(4) = 85
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)28;

    /* Compute a(4) from the recurrence. */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)9;
    seq_t a3 = (seq_t)28;
    seq_t a4 = 2 * a3 + 4 * a2 - 2 * a1 - a0;
    if (n == 4) return a4;

    /* Sliding window for the recurrence. */
    seq_t v0 = a1; /* a(i-4) when i = 5 */
    seq_t v1 = a2; /* a(i-3) */
    seq_t v2 = a3; /* a(i-2) */
    seq_t v3 = a4; /* a(i-1) */
    seq_t v4;      /* a(i) */

    for (idx_t i = 5; i <= n; ++i) {
        v4 = 2 * v3 + 4 * v2 - 2 * v1 - v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
    }
    return v4;
}
