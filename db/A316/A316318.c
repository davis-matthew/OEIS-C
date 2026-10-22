/*
 * A316318  Coordination sequence for a node in the graph of Balaban's
 *          (3,10)-cage.
 * https://oeis.org/A316318
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

/* Coordination sequence for Balaban's (3,10)-cage:
   a(0)=1, a(1)=3, a(2)=6, a(3)=12, a(4)=24,
   a(5)=20, a(6)=4, and a(n)=0 for n>6. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)24;
    if (n == 5) return (seq_t)20;
    if (n == 6) return (seq_t)4;
    return (seq_t)0;
}
