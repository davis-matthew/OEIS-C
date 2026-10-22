/*
 * A359452  Number of vertices in the partite set of the n-Menger sponge graph
 *          that contains the corners.
 * https://oeis.org/A359452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20^n + (-4)^n)/2 Recurrence: a(0)=1, a(1)=8, a(n) =
 *            16*a(n-1) + 80*a(n-2) for n ≥ 2 (derived from the ordinary
 *            generating function (1-8x)/((1-20x)(1+4x)))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * (seq_t)16 + prev2 * (seq_t)80;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
