/*
 * A313876  Coordination sequence Gal.6.342.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5, for n ≥ 2: a(n) = 5*n + 3*⌊(n-2)/11⌋ + [
 *            (n-2)%11 > 0 ] + [ (n-2)%11 > 3 ] + [ (n-2)%11 > 6 ]. Here [P]
 *            is 1 if predicate P is true, 0 otherwise
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    idx_t m = n - 2;              /* n ≥ 2 so m ≥ 0 */
    idx_t full_cycles = m / 11;   /* number of complete 11‑element blocks */
    idx_t remainder   = m % 11;   /* length of the last incomplete block */

    /* Count the “+1” extras contributed by the special indices
       3, 6, 9, 14, 17, 20, … (gap pattern 3,3,5). */
    seq_t extras = (seq_t)3 * (seq_t)full_cycles;
    if (remainder > 0) extras += (seq_t)1;   /* index 0 within block */
    if (remainder > 3) extras += (seq_t)1;   /* index 3 within block */
    if (remainder > 6) extras += (seq_t)1;   /* index 6 within block */

    return (seq_t)5 * (seq_t)n + extras;
}
