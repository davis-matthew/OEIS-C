/*
 * A350979  a(0)=1, a(1)=652, thereafter a(n) = 254*a(n-1)-a(n-2)+378.
 * https://oeis.org/A350979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=652, a(n)=254*a(n-1)-a(n-2)+378 for n>=2 */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)652;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)652;
    seq_t cur;

    for (idx_t i = 2u; i <= n; ++i)
    {
        cur = (seq_t)254 * prev1 - prev2 + (seq_t)378;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
