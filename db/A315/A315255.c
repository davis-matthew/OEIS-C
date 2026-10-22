/*
 * A315255  Coordination sequence Gal.5.67.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-10)+48 for n ≥ 10, with base values a(0)=1, a(1)=6,
 *            a(2)=10, a(3)=15, a(4)=20, a(5)=24, a(6)=28, a(7)=33, a(8)=38,
 *            a(9)=42. The sequence is thus a(n) = base[n%10] + 48*(n/10) for
 *            n > 0; a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    static const seq_t base[10] = {0, 6, 10, 15, 20, 24, 28, 33, 38, 42};
    idx_t q = n / 10;
    unsigned r = (unsigned)(n % 10);
    return base[r] + (seq_t)q * 48;
}
