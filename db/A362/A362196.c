/*
 * A362196  Number of Grassmannian permutations of size n that avoid a
 *          pattern, sigma, where sigma is a pattern of size 9 with exactly
 *          one descent.
 * https://oeis.org/A362196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + ∑_{i=2..8} binomial(n,i)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of Grassmannian permutations of size n avoiding a specific
 * pattern σ of length 9 with exactly one descent.
 *
 * The closed‑form is
 *
 *     a(n) = 1 + Σ_{i=2}^{8} C(n,i)          (for all n ≥ 0)
 *
 * where C(n,i) denotes the binomial coefficient.  For n < 2 the sum is empty,
 * hence a(0)=a(1)=1.
 *
 * The implementation evaluates the sum incrementally using the identity
 *
 *     C(n,i+1) = C(n,i) * (n-i) / (i+1)
 *
 * which keeps intermediate values small enough for every seq_t that can hold
 * the exact result.
 */

seq_t a(idx_t n)
{
    /* a(0) = a(1) = 1 */
    if (n < 2)
        return 1;

    seq_t result = 1;
    /* C(n,2) */
    seq_t binom = (seq_t)n * (seq_t)(n - 1) / 2;
    result += binom;

    for (idx_t i = 3; i <= 8; ++i) {
        /* C(n,i) from C(n,i-1) */
        binom = binom * (seq_t)(n - (i - 1)) / (seq_t)i;
        result += binom;
    }
    return result;
}
