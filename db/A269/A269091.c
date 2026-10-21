/*
 * A269091  Number of n X 2 0..3 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling three
 *          exactly once.
 * https://oeis.org/A269091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4 a(n) = 96 * 7^(n-3) * (5*n-3) for n > 1 (OEIS A269091)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)             /* offset is 1; out‑of‑range request */
        return (seq_t)0;

    if (n == 1)             /* special case */
        return (seq_t)4;

    if (n == 2)             /* formula works for n=2 as well, but keep
                               it explicit to avoid negative exponent   */
        return (seq_t)96;

    if (n == 3)
        return (seq_t)1152;

    /* n ≥ 4: use the closed form */
    idx_t k = n - 3;                /* exponent of 7 (non‑negative)     */
    seq_t pow7 = (seq_t)1;

    for (idx_t i = 0; i < k; ++i)
        pow7 *= (seq_t)7;          /* compute 7^(n-3)                   */

    seq_t coeff = (seq_t)(5 * (long long)n - 3);  /* 5*n-3                */

    return (seq_t)96 * pow7 * coeff;
}
