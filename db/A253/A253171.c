/*
 * A253171  a(n) = number of permutations of (1,2,...,n) producible by an
 *          ordered triple of distinct transpositions.
 * https://oeis.org/A253171
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n^5 - 7*n^4 + 17*n^3 - 17*n^2 + 30*n - 24) / 48
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 49, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for all n ≥ 3 and is O(1). */
seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t N = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */
    seq_t n5 = n4 * N;         /* n^5 */

    /* Compute the polynomial inside the parentheses */
    seq_t term = n5;                                   /* n^5 */
    term = term - (seq_t)7 * n4;                       /* - 7*n^4 */
    term = term + (seq_t)17 * n3;                      /* + 17*n^3 */
    term = term - (seq_t)17 * n2;                      /* - 17*n^2 */
    term = term + (seq_t)30 * N;                       /* + 30*n  */
    term = term - (seq_t)24;                           /* - 24   */

    /* Multiply by n and divide by 48 */
    seq_t result = (N * term) / (seq_t)48;
    return result;
}
