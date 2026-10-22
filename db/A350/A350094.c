/*
 * A350094  a(n) = Sum_{k=0..n} n CNIMPL k where CNIMPL = NOT(n) AND k is the
 *          bitwise logical converse non-implication operator (A102037).
 * https://oeis.org/A350094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A350094:  a(0) = 0
   a(2*n)   = 4*a(n) + n
   a(2*n+1) = 4*a(n)
   and a(n) is the sum of (~n & k) for k = 0..n.
   The recurrence follows directly from the definition, and
   computing it recursively is O(log n) for any n.
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t m = n >> 1;          /* floor(n/2) */
    seq_t sub = a(m);          /* a(m) */
    if ((n & 1) == 0)          /* n is even: n = 2*m */
        return 4 * sub + (seq_t)m;
    else                       /* n is odd: n = 2*m + 1 */
        return 4 * sub;
}
