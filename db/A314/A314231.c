/*
 * A314231  Coordination sequence Gal.5.305.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1 let m = n % 9. If n==1 or m==5, a(n)=a(n-1)+4;
 *            else if m==0 or m==1, a(n)=a(n-1)+5; else a(n)=a(n-1)+6
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t inc;
        if (i == 1) {
            inc = 4;
        } else {
            idx_t m = i % 9;
            if (m == 5) {
                inc = 4;
            } else if (m == 0 || m == 1) {
                inc = 5;
            } else {
                inc = 6;
            }
        }
        res += inc;
    }
    return res;
}
