/*
 * A328144  Orders of cubic (i.e., trivalent, 3-regular) distance-transitive
 *          connected graphs, from smallest to largest, with duplications when
 *          there exist multiple graphs with the same order.
 * https://oeis.org/A328144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A328144: Orders of cubic (3‑regular) distance‑transitive connected graphs.
   The sequence is known and finite.  Values are stored in a static table
   and returned with 1‑based indexing.  For n > 12 the function returns 0. */

seq_t a(idx_t n)
{
    static const seq_t vals[12] = {
        4,  /* K4 */
        6,  /* K3,3 */
        8,  /* hypercube Q3 */
        10, /* Petersen graph */
        14, /* Heawood graph */
        18, /* generalized Petersen G(9,2) */
        20, /* dodecahedron graph */
        20, /* Desargues graph */
        28, /* Tutte–Coxeter graph */
        30, /* cubic distance‑transitive graph of order 30 */
        90, /* Heawood bipartite? (Fowler–Fulkerson) */
        102 /* Biggs–Smith graph */
    };

    if (n == 0) return (seq_t)0;   /* Undefined for n = 0 */
    if (n <= 12) return vals[n - 1];
    return (seq_t)0;               /* No further terms defined */
}
