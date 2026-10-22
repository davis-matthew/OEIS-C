/*
 * A331063  Coordination sequence for trivalent vertex in 1-skeleton of
 *          rhombic triacontahedron.
 * https://oeis.org/A331063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Coordination sequence for a trivalent vertex in the
   1‑skeleton of a rhombic triacontahedron.  The graph has
   diameter 6, and the counts at each distance are
   a(0)=1, a(1)=3, a(2)=9, a(3)=6, a(4)=9,
   a(5)=3, a(6)=1.  All larger distances are 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)9;
    if (n == 5) return (seq_t)3;
    if (n == 6) return (seq_t)1;
    return (seq_t)0;
}
