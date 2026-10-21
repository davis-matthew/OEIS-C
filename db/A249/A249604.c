/*
 * A249604  a(n) = Sum_{i=1..n} Fibonacci(i)*10^(i-1).
 * https://oeis.org/A249604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249604: a(n) = Sum_{i=1..n} Fib(i)*10^(i-1)
   Recurrence: a(1)=1, a(2)=11, a(3)=211
   a(n) = 11*a(n-1) + 90*a(n-2) - 100*a(n-3)  for n > 3
*/

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0)               /* out of offset, return 0 */
        return 0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)11;
    if (n == 3)
        return (seq_t)211;

    /* Keep the last three computed values. */
    seq_t a1 = (seq_t)1;      /* a(n-3) */
    seq_t a2 = (seq_t)11;     /* a(n-2) */
    seq_t a3 = (seq_t)211;    /* a(n-1) */

    /* Iterate up to n, computing each successive term. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 11 * a3 + 90 * a2 - 100 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
