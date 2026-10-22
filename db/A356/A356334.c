/*
 * A356334  a(n) is the number of nonnegative integer solutions (x; y) with x
 *          <= y of x^(n+1) + y^(n+1) = (x+y)^n.
 * https://oeis.org/A356334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Number of non‑negative integer solutions (x ≤ y) of
 *  x^(n+1) + y^(n+1) = (x + y)^n.  The only solutions are:
 *      (0,0), (0,1), (2^(n−1),2^(n−1)) for every n > 0,
 *  and additionally (1,2) for n = 2.
 *  Hence a(0)=1, a(1)=3, a(2)=4, and a(n)=3 for all n ≥ 3.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    return (seq_t)3;
}
