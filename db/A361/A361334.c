/*
 * A361334  Index of 2^n in A351495.
 * https://oeis.org/A361334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 2 a(2) = 4 a(3) = 7 a(4) = 16 For n >= 5: if n
 *            is even: a(n) = 2 * a(n-1) if n is odd: a(n) = 2 * a(n-1) -
 *            offset offset = 6 when (n-5)/2 is even 4 when (n-5)/2 is odd
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)4;
    if (n == 3u) return (seq_t)7;
    if (n == 4u) return (seq_t)16;

    seq_t prev = (seq_t)16;      /* a(4) */
    for (idx_t i = 5u; i <= n; ++i)
    {
        if ((i & 1u) == 0u)          /* even i */
        {
            prev = prev * (seq_t)2u;
        }
        else                         /* odd i */
        {
            idx_t k = (i - 5u) / 2u; /* (n-5)/2 */
            seq_t offset = (((k & 1u) == 0u) ? (seq_t)6u : (seq_t)4u);
            prev = prev * (seq_t)2u - offset;
        }
    }
    return prev;
}
