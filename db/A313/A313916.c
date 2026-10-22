/*
 * A313916  Coordination sequence Gal.5.293.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(27*n/5) + d(n) where d(n) = 1 if n≡4 (mod5), n = 0,
 *            or n≡2,3 (mod5) and ((n-m)/5) is odd; otherwise d(n)=0
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)27 * n) / 5;   /* floor(27n/5) */
    unsigned d;

    switch (n % 5) {
    case 0:
        d = (n == 0) ? 1 : 0;
        break;
    case 1:
        d = 0;
        break;
    case 2:
        d = (((n - 2) / 5) % 2) ? 1 : 0;
        break;
    case 3:
        d = (((n - 3) / 5) % 2) ? 1 : 0;
        break;
    case 4:
        d = 1;
        break;
    default:
        d = 0;   /* unreachable */
    }

    return base + d;
}
