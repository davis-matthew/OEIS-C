/*
 * A332558  a(n) is the smallest k such that n*(n+1)*(n+2)*...*(n+k) is
 *          divisible by n+k+1.
 * https://oeis.org/A332558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the smallest k such that n · (n+1) · … · (n+k) is
 *            divisible by n+k+1 (OEIS A332558, offset 1)
 * Range:     u8 n <= 797, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = 0;
    for (;; ++k) {
        /* Target divisor M = n + k + 1. */
        seq_t M = (seq_t)n + (seq_t)k + 1;

        /* Compute the product (n)(n+1)…(n+k) modulo M. */
        seq_t prod = 1;
        for (idx_t i = 0; i <= k; ++i) {
            prod = (prod * ((seq_t)n + (seq_t)i)) % M;
        }

        /* If the product is 0 mod M, we found the minimal k. */
        if (prod == 0) {
            return (seq_t)k;
        }
    }
}
