/*
 * A368292  Expansion of e.g.f. exp(x) / (1 - x*exp(3*x)).
 * https://oeis.org/A368292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Σ_{k=0..n} (3*(n-k)+1)^k / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum     = 0;     /* accumulator for the sum */
    seq_t product = 1;     /* n! / k! for the current k (descending) */

    for (idx_t k = n; ; --k) {
        /* base = 3*(n-k)+1, computed with the original n */
        seq_t base = (seq_t)((idx_t)3 * (n - k) + (idx_t)1);

        /* Compute base^k by repeated multiplication */
        seq_t power = 1;
        for (idx_t e = 0; e < k; ++e) {
            power *= base;
        }

        /* Add current term to the sum: (base^k) * (n! / k!) */
        sum += power * product;

        /* Prepare product for the next iteration (k-1) */
        if (k == 0) break;           /* finished last term */
        product *= (seq_t)k;         /* product *= k  -> n!/(k-1)! */
    }

    return sum;
}
