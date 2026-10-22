/*
 * A367546  a(n) = Sum_{k=0..n} n^k * |(n - k | k)|, where (a | b) denotes the
 *          Kronecker symbol.
 * https://oeis.org/A367546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} [gcd(k, n) = 1] * n^k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (equivalently, sum of n^k over k coprime to n) */

seq_t a(idx_t n)
{
    /* Handle the offset and trivial cases */
    if (n == 0)
        return 0;        /* a(0) = 0 */

    if (n == 1)
        return 2;        /* 1^0 + 1^1 = 1 + 1 = 2 */

    /* Compute the sum n^k for k = 1 .. n-1 with gcd(k, n) == 1 */
    seq_t sum = 0;
    seq_t pow = (seq_t)n;   /* n^1 */

    for (idx_t k = 1; k < n; ++k) {
        /* Euclidean gcd of k and n */
        idx_t a = k, b = n;
        while (b != 0) {
            idx_t r = a % b;
            a = b;
            b = r;
        }
        if (a == 1)          /* gcd(k, n) == 1 */
            sum += pow;

        /* Prepare pow for next exponent: n^{k+1} = n^k * n */
        pow *= (seq_t)n;
    }

    return sum;
}
