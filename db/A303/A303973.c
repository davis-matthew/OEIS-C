/*
 * A303973  Total volume of all rectangular prisms with dimensions (p,p,q)
 *          such that n = p + q, p divides q and p < q.
 * https://oeis.org/A303973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊(n-1)/2⌋} i^2 * (n-i) * (⌊(n-i)/i⌋ -
 *            ⌊(n-i-1)/i⌋)
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;
    seq_t res = 0;
    idx_t max_i = (n - 1) / 2;          /* upper bound for i */
    for (idx_t i = 1; i <= max_i; ++i)
    {
        idx_t diff = n - i;             /* n - i  */
        if (diff % i == 0)              /* i divides n-i ?  */
        {
            seq_t term = (seq_t)i * (seq_t)i;   /* i^2   */
            term *= (seq_t)diff;                /* * (n-i) */
            res += term;
        }
    }
    return res;
}
