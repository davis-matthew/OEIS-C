/*
 * A246081  Paradigm shift sequence for (0,3) production scheme with
 *          replacement.
 * https://oeis.org/A246081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-9) for n >= 15, with the first 14 terms given by
 *            a(1)=1,a(2)=2,...,a(12)=12,a(13)=14,a(14)=16
 * Range:     u8 n <= 36, u16 n <= 81, u32 n <= 173, u64 n <= 354,
 *            u128 n <= 717, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry A246081 – Paradigm shift sequence for (0,3) production scheme with replacement. */

seq_t a(idx_t n)
{
    /* First 14 terms as a constant static array */
    static const seq_t base[14] =
    {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16
    };
    /* For indices 1..14 directly lookup */
    if (n <= 14)
        return base[n - 1];

    /* For n >= 15 compute by repeated application of a(n)=3*a(n-9) */
    idx_t t   = n;
    seq_t mul = 1;
    /* t is unsigned, loop only while t > 14 to avoid underflow */
    while (t > 14)
    {
        mul *= 3;
        t   -= 9;
    }
    /* t now lies in 1..14, return multiplier * base[t-1] */
    return mul * base[t - 1];
}
