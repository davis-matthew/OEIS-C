/*
 * A369180  Alternating sum of the k-adic valuations (ruler functions) of n.
 * https://oeis.org/A369180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=2..n} (-1)^k * valuation(n,k)
 * Range:     i8 n <= 9513, i16 n <= 9513, i32 n <= 9513, i64 n <= 9513,
 *            i128 n <= 9513, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t sum = 0;
    for (idx_t k = 2; k <= n; ++k) {
        idx_t temp = n;
        seq_t e = 0;
        while (temp % k == 0) {
            temp /= k;
            e += 1;
        }
        if ((k & 1) == 0)   /* even k : sign +1 */
            sum += e;
        else                /* odd k : sign -1 */
            sum -= e;
    }
    return sum;
}
