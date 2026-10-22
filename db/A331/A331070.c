/*
 * A331070  Coordination sequence for pentavalent vertex in 1-skeleton of
 *          pentagonal hexecontahedron.
 * https://oeis.org/A331070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A331070 (Pentagonal hexecontahedron coordination sequence):
   The graph is finite with 92 vertices.  The sequence is
   1,5,10,10,15,20,10,10,10,1,0,0,0,…  thus all terms beyond index 9 are 0. */

seq_t a(idx_t n)
{
    static const seq_t table[10] = {1,5,10,10,15,20,10,10,10,1};
    return (n < 10) ? table[n] : 0;
}
