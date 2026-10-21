/*
 * A270369  Expansion of g.f. (1-7*x)/(1-9*x).
 * https://oeis.org/A270369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270369: Expansion of g.f. (1-7*x)/(1-9*x)
   Formula (recurrence): a(0)=1; a(1)=2; a(n)=9*a(n-1) for n>1.
   Equivalent explicit formula: a(0)=1; a(n)=2*9^(n-1) for n>0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t value = (seq_t)2;          /* a(1) = 2 */
    for (idx_t i = 1; i < n; ++i)
        value = value * (seq_t)9;     /* multiply by 9 each step */
    return value;
}
