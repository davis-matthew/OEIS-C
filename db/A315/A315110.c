/*
 * A315110  Coordination sequence Gal.6.555.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(16*n/3) + g(n mod 6) for n >= 6, with g =
 *            [-1,1,-1,0,1,2]; a(0)=1, a(1)=5, a(2)=9, a(3)=16, a(4)=22,
 *            a(5)=27
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[6] = {1, 5, 9, 16, 22, 27};
    if (n < 6)
        return init[n];
    seq_t base = ((seq_t)n * 16) / 3;          /* floor(16*n/3) */
    static const int8_t offs[6] = { -1, 1, -1, 0, 1, 2 };
    long long temp = (long long)base + offs[n % 6];
    return (seq_t)temp;
}
