/*
 * A329772  Coordination sequence for 10-valent vertex in 1-skeleton of
 *          triakis icosahedron.
 * https://oeis.org/A329772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, bref
 * Formula:   a(0)=1; a(1)=10; a(2)=15; a(3)=6; a(n)=0 for n>=4
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts vertices at graph distance n from a 10‑valent vertex
   in the triakis icosahedron.  The graph has diameter 3, so all terms
   beyond n=3 are zero. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 10;
    if (n == 2) return 15;
    if (n == 3) return 6;
    return 0;
}
