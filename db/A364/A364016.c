/*
 * A364016  Expansion of Sum_{k>0} k^2 * x^(2*k) / (1 + x^(3*k)).
 * https://oeis.org/A364016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, n/d==2 (mod 3)} (-1)^(n/d) * d^2
 * Range:     i8 n <= 23, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by iterating over the divisors of n.  */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over all divisor pairs (d, n/d) with d <= n/d */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;          /* d is not a divisor */

        /* first divisor: t1 = d, corresponding d1 = n/d */
        idx_t t1 = d;
        idx_t d1 = n / d;
        if (t1 % 3 == 2) {
            seq_t sq = (seq_t)d1 * d1;
            sum += ((t1 & 1) ? -1 : 1) * sq;   /* (-1)^t1 * d1^2 */
        }

        /* second divisor: t2 = n/d, corresponding d2 = d */
        if (d != n / d) {                    /* avoid double counting square root */
            idx_t t2 = n / d;
            idx_t d2 = d;
            if (t2 % 3 == 2) {
                seq_t sq = (seq_t)d2 * d2;
                sum += ((t2 & 1) ? -1 : 1) * sq;   /* (-1)^t2 * d2^2 */
            }
        }
    }
    return sum;
}
