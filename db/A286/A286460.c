/*
 * A286460  Compound filter (2-adic valuation & sum of the divisors): a(n) =
 *          P(A001511(n), A000203(n)), where P(n,k) is sequence A000027 used
 *          as a pairing function.
 * https://oeis.org/A286460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A001511(n)+A000203(n))^2) - A001511(n) -
 *            3*A000203(n))
 * Range:     u8 n <= 7, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Off‑offset behaviour is undefined; return 0 for safety. */
        return (seq_t)0;
    }

    /* Compute A001511(n): v₂(n)+1 (exponent of the highest power of 2 dividing n plus 1). */
    idx_t temp = n;
    idx_t v2 = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        v2++;
    }
    seq_t a_val = (seq_t)(v2 + 1);

    /* Compute A000203(n): sum of all positive divisors of n. */
    seq_t sigma = 0;
    for (idx_t d = 1; d <= n / d; ++d) {      /* loop until d > n/d */
        if (n % d == 0) {
            sigma += (seq_t)d;
            idx_t other = n / d;
            if (other != d) {
                sigma += (seq_t)other;
            }
        }
    }

    /* Apply the given pairing function formula. */
    seq_t sum = a_val + sigma;
    seq_t sq = sum * sum;
    seq_t numerator = (seq_t)2 + sq - a_val - (seq_t)3 * sigma;
    seq_t result = numerator / 2;

    return result;
}
