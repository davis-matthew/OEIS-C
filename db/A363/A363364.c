/*
 * A363364  Least nonnegative integer k such that all non-bipartite graphs
 *          with n nodes and at least k edges are weakly pancyclic.
 * https://oeis.org/A363364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 for n ≤ 5; 14 for n = 8; otherwise a(n) =
 *            floor((n-1)*(n-3)/4) + 5
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≤ 5 all graphs are weakly pancyclic. */
    if (n <= 5) {
        return 0;
    }

    /* The only known exceptional case is n = 8. */
    if (n == 8) {
        return 14;
    }

    /* For n ≥ 6, n ≠ 8, the value follows the proven lower bound:
       floor((n-1)*(n-3)/4) + 5. */
    seq_t tmp = (seq_t)(n - 1) * (seq_t)(n - 3);
    return (tmp / 4) + 5;
}
