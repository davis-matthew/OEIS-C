/*
 * A313103  Coordination sequence Gal.6.260.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=9, a(3)=14; for n>=4, a(n)=19 +
 *            58*((n-4)/12) + pre[(n-4)%12] where pre =
 *            {0,4,10,16,20,25,30,35,39,43,48,53,58}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)14;

    static const seq_t pre[13] = {0,4,10,16,20,25,30,35,39,43,48,53,58};

    idx_t k = n - 4;
    idx_t cycles = k / 12;
    idx_t rem = k % 12;
    seq_t result = (seq_t)19;
    result += (seq_t)cycles * (seq_t)58;
    result += pre[rem];
    return result;
}
