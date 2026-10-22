/*
 * A329499  Coordination sequence for 1-skeleton of the 24-cell (or {3,4,3}).
 * https://oeis.org/A329499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, bref
 * Formula:   a(0)=1, a(1)=8, a(2)=14, a(3)=1, and a(n)=0 for all n>3
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)1;
    return (seq_t)0;
}
