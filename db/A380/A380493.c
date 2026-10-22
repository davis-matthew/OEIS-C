/*
 * A380493  a(n) = n! * Sum_{k=0..n} binomial(2*n+3,k)/(n-k)!.
 * https://oeis.org/A380493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} binomial(2*n+3, k)/(n-k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* running sum of terms */
    seq_t bin = 1;          /* binomial coefficient C(2*n+3, k) */
    seq_t prod = 1;         /* product n!/(n-k)! for current k */
    seq_t tn = n;           /* n cast to seq_t for arithmetic */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Update the product: multiply by (n - k + 1) */
            prod *= (seq_t)(n - k + 1);

            /* Update binomial coefficient:
               bin = bin * (2*n + 4 - k) / k   (division is exact) */
            seq_t kt = (seq_t)k;
            bin = (bin * (tn * 2 + 4 - kt)) / kt;
        }
        sum += bin * prod;
    }
    return sum;
}
