/*
 * A267984  Numbers congruent to {17, 23} mod 30.
 * https://oeis.org/A267984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (30*n - 9*(-1)^n - 5) / 2 (n ≥ 1) Equivalent piecewise:
 *            if n even → a(n) = 15*n - 7 if n odd → a(n) = 15*n + 2 This
 *            implements the exact closed‑form expression for OEIS A267984
 *            (numbers congruent to 17 or 23 mod 30)
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; the expression works for any n >= 1 */
    /* Compute 15*n in the wider idx_t type to avoid overflow in seq_t */
    idx_t prod = (idx_t)15 * n;

    if ((n & 1) == 0)          /* n even → a(n) = 15*n - 7 */
    {
        return (seq_t)prod - (seq_t)7;
    }
    else                         /* n odd  → a(n) = 15*n + 2 */
    {
        return (seq_t)prod + (seq_t)2;
    }
}
