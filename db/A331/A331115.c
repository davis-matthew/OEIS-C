/*
 * A331115  Numerators of the best approximations for sqrt(2).
 * https://oeis.org/A331115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1) = 1, a(2) = 3 for n > 2 if n mod 3 == 2: a(n) = 3*a(n-1) -
 *            a(n-2) else : a(n) = a(n-1) + a(n-2)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 76, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)             return (seq_t)0; /* undefined, safe fallback */
    if (n == 1)             return (seq_t)1;
    if (n == 2)             return (seq_t)3;

    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)3; /* a(n-1) */
    seq_t cur   = prev1;    /* will hold a(n) */

    for (idx_t k = 3; k <= n; ++k)
    {
        if (k % 3 == 2)
            cur = (seq_t)3 * prev1 - prev2;
        else
            cur = prev1 + prev2;

        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
