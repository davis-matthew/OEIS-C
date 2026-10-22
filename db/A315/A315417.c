/*
 * A315417  Coordination sequence Gal.6.354.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = base[n%12] + 62*(n/12)  if n>0  and n%12!=0 */
/*          a(n) = 62*(n/12)                if n>0  and n%12==0 */
/* where base = {1,6,11,15,20,26,31,36,42,47,51,56}. */

seq_t a(idx_t n)
{
    const seq_t base[12] = {1,6,11,15,20,26,31,36,42,47,51,56};
    if (n == 0) return 1;                /* a(0) */
    const idx_t q = n / 12;
    const idx_t r = n % 12;
    if (r == 0) return (seq_t)q * 62;    /* multiples of 12, n>0 */
    return base[r] + (seq_t)q * 62;      /* other indices */
}
