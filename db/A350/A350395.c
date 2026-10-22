/*
 * A350395  Numbers m such that a term with the largest coefficient in
 *          Product_{k=1..m} (1 + x^k) is unique.
 * https://oeis.org/A350395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n=1, 3 for n=2, and for n≥3 a(n) = 4 * (⌊(n-3)/2⌋ +
 *            2) + (n-3 mod 2) * 3 Equivalently: list all numbers m≥8 such
 *            that m mod 4 is 0 or 3, plus 0 and 3
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)3;

    /* For n ≥ 3 */
    seq_t offset = (seq_t)(n - (idx_t)3);          /* (n-3)  */
    seq_t k      = offset / (seq_t)2 + (seq_t)2;   /* ⌊(n-3)/2⌋ + 2 */

    if ((offset & (seq_t)1) == (seq_t)0)          /* offset even */
        return (seq_t)4 * k;                     /* 4k   */
    else
        return (seq_t)4 * k + (seq_t)3;          /* 4k+3 */
}
