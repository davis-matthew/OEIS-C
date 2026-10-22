/*
 * A363964  Number of unordered pairs of disjoint self-avoiding paths with
 *          nodes that cover all vertices of a convex labeled n-gon; one-node
 *          paths are allowed.
 * https://oeis.org/A363964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(n)=n*(n-1)*(n^2+n+36)*2^(n-8)/3 for n>=3
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 23, u64 n <= 50, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first two terms */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Components of the formula */
    seq_t a_part = (seq_t)n;
    seq_t b_part = (seq_t)(n - 1);
    seq_t c_part = (seq_t)n * (seq_t)n + (seq_t)n + (seq_t)36;

    /* Compute the product and divide by 3 */
    seq_t prod = a_part * b_part;
    prod = prod * c_part;
    prod /= (seq_t)3;   /* exact division */

    /* Multiply or divide by 2^(n-8) via shifts */
    if (n >= 8)
        prod <<= (n - 8);          /* multiply by 2^(n-8) */
    else
        prod >>= (8 - n);          /* divide by 2^(8-n) */

    return prod;
}
