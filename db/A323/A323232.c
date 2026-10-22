/*
 * A323232  a(n) = 2^n*J(n, 1/2) where J(n, x) are the Jacobsthal polynomials
 *          as defined in A322942.
 * https://oeis.org/A323232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 3 a(2) = 9 a(n) = 3*a(n-1) + 8*a(n-2) for n >= 3
 *            (recurrence derived from the OEIS definition)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* a(1) and a(2) are the two previous terms */
    seq_t prev2 = (seq_t)3;  /* a(n-2) when n == 3 */
    seq_t prev1 = (seq_t)9;  /* a(n-1) when n == 3 */
    seq_t cur = 0;          /* will hold a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)3 * prev1 + (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
