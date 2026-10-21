/*
 * A253387  A "mod sequence" where a(n) is the eventual constant value
 *          attained by the sequence defined as b(1) = n, b(m) =
 *          (Sum_{k=1..m-1} b(k)) mod m.
 * https://oeis.org/A253387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the eventual constant value attained by the sequence
 *            defined by b(1) = n, b(m) = (sum_{k=1}^{m-1} b(k)) mod m. The
 *            constant is found when (sum_{k=1}^{m-1} b(k)) / m == b(m), i.e.
 *            the quotient equals the remainder. This condition is equivalent
 *            to sum_{k=1}^{m-1} b(k) = b(m)*(m+1)
 * Range:     i8 none, i16 none, i32 n <= 1000, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t T = (seq_t)n;           /* Sum of b(1)..b(m-1); initially b(1)=n */
    for (idx_t m = 2; ; ++m) {
        seq_t b = T % (seq_t)m;   /* b(m) = T mod m                      */
        seq_t q = T / (seq_t)m;   /* Quotient of T by m                  */
        if (b == q)               /* Condition met → constant reached    */
            return b;
        T += b;                   /* Update running sum for next step    */
    }
}
