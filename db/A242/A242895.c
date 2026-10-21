/*
 * A242895  Beginning with a central 'Sun' configuration of a Penrose 'Kite'
 *          and 'Dart' tiling with rotational symmetry as the first step,
 *          number of tiles that can be added to the free edges of the current
 *          tiling.
 * https://oeis.org/A242895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=5, a(2)=5, a(n)=5*(n-1) for n>=3
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2ULL)
        return (seq_t)5;
    return (seq_t)5 * ((seq_t)n - (seq_t)1);
}
