/*
 * A289223  Number of ways to select 2 disjoint point triples from an n X n X
 *          n triangular point grid, each point triple forming an 2 X 2 X 2
 *          triangle.
 * https://oeis.org/A289223
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n^4 - 4*n^3 - 7*n^2 + 46*n - 48)/2 for n >= 2
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 304, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0 as a safe placeholder. */
    if (n < 2) {
        return 0;
    }

    /* Work in seq_t to avoid overflow and to accommodate arbitrary precision. */
    seq_t nn = (seq_t)n;                /* n */
    seq_t n2 = nn * nn;                /* n^2 */
    seq_t n3 = n2 * nn;                /* n^3 */
    seq_t n4 = n2 * n2;                /* n^4 */

    /* Compute a1 = n^4 + 46*n - 48   and a2 = 4*n^3 + 7*n^2 */
    seq_t a1 = n4 + ((seq_t)46 * nn) - (seq_t)48;
    seq_t a2 = ((seq_t)4 * n3) + ((seq_t)7 * n2);

    /* The result is (a1 - a2) / 2. a1 >= a2 for all n >= 2. */
    seq_t diff = a1 - a2;
    return diff / 2;
}
