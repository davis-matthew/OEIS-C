/*
 * A371154  Maximum number of vertices for a given diameter n of a Cayley
 *          digraph on the cyclic group with generators s=1 and t>s.
 * https://oeis.org/A371154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n+2)^2/3)-1 for n≠1, special case a(1)=3
 * Range:     u8 n <= 13, u16 n <= 440, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;
    seq_t ns = (seq_t)n;
    seq_t v  = (ns + 2) * (ns + 2);      // (n+2)^2
    seq_t t  = (v + 2) / 3;              // ceil division of v by 3
    return t - 1;
}
