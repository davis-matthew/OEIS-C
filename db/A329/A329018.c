/*
 * A329018  a(n) = p(1,n), where p(x,n) is the strong divisibility sequence of
 *          polynomials based on sqrt(3/2) as in A328644.
 * https://oeis.org/A329018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n ≥ 2, if n ≡ 1 (mod 5) : a(n) = 30 * a(n‑1) + 1 if
 *            n ≡ 0 (mod 5) : a(n) = (6 * a(n‑1)) / 5 + 1 otherwise : a(n) = 6
 *            * a(n‑1) + 1 This recurrence generates the sequence A329018
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t prev = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        switch ((i % 5U))
        {
        case 0: /* n ≡ 0 (mod 5) */
            prev = (prev * (seq_t)6) / (seq_t)5 + (seq_t)1;
            break;
        case 1: /* n ≡ 1 (mod 5) */
            prev = prev * (seq_t)30 + (seq_t)1;
            break;
        default: /* n ≡ 2,3,4 (mod 5) */
            prev = prev * (seq_t)6 + (seq_t)1;
            break;
        }
    }
    return prev;
}
