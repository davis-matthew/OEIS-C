/*
 * A324658  a(n) = n - A324659(n), where A324659(n) is half of bitwise-AND of
 *          2*n and sigma(n).
 * https://oeis.org/A324658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n - ((2*n) & sigma(n))/2
 * Range:     u8 n <= 254, u16 n <= 9692, u32 n <= 9692, u64 n <= 9692,
 *            u128 n <= 9692, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute σ(n), the sum of all positive divisors of n. */
    seq_t sigma = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            seq_t j = (seq_t)(n / i);
            sigma += (seq_t)i;
            if (j != i) {
                sigma += j;
            }
        }
    }

    /* Compute the required value: n - (((2*n) & σ(n)) / 2). */
    seq_t two_n   = (seq_t)n * 2;
    seq_t and_val = two_n & sigma;
    seq_t half    = and_val >> 1;   /* (and_val) / 2 with truncation. */
    return (seq_t)n - half;
}
