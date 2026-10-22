/*
 * A308985  Expansion of Product_{k>=0} (1 + 2*x^(2^k))^2.
 * https://oeis.org/A308985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(popcount(k)+popcount(n-k))
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1;
   if (n is odd)  a(n) = 4 * a((n-1)/2);
   if (n is even) a(n) = a(n/2) + 4 * a((n-2)/2); */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n & 1) {                     /* n is odd: 2k+1 */
        return (seq_t)4 * a((n - 1) >> 1);
    } else {                          /* n is even: 2k   */
        return a(n >> 1) + (seq_t)4 * a((n - 2) >> 1);
    }
}
