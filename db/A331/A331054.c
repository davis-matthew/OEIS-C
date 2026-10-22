/*
 * A331054  Coordination sequence for tetravalent vertex in 1-skeleton of
 *          disdyakis dodecahedron.
 * https://oeis.org/A331054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence for tetravalent vertex in 1‑skeleton of
 * disdyakis dodecahedron.  The graph is finite; after the maximum
 * graph distance (4) all further terms are zero. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)4;
    if (n == (idx_t)2) return (seq_t)12;
    if (n == (idx_t)3) return (seq_t)8;
    if (n == (idx_t)4) return (seq_t)1;
    /* All vertices at distance >4 do not exist in the 1‑skeleton. */
    return (seq_t)0;
}
