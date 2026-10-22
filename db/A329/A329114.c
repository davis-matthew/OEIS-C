/*
 * A329114  a(n) = floor(A026532(n)/5).
 * https://oeis.org/A329114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; a(2)=0; for n>=2, a(n+1)=3*a(n) if n is odd, otherwise
 *            a(n+1)=2*a(n)+1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    seq_t cur = (seq_t)0;          /* a(2) is 0 */
    for (idx_t i = 2; i < n; ++i)  /* i is the current index (a(i)) */
    {
        if ((i & 1u) == 1u)        /* i odd  -> a(i+1)=3*a(i) */
            cur = 3 * cur;
        else                      /* i even -> a(i+1)=2*a(i)+1 */
            cur = 2 * cur + 1;
    }
    return cur;                   /* cur == a(n) */
}
