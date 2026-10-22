/*
 * A315061  Coordination sequence Gal.5.130.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=5*n+δ(n) for n≥1, where δ(n) = +1 if n≡6 (mod 8),
 *            -1 if n≡2 (mod 8), 0 otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;                /* a(0) = 1 */
    seq_t val = (seq_t)5 * n;            /* 5*n */
    switch (n & 7) {                     /* n % 8 */
        case 2:                          /* n≡2 (mod 8): δ = –1 */
            val -= (seq_t)1;
            break;
        case 6:                          /* n≡6 (mod 8): δ = +1 */
            val += (seq_t)1;
            break;
        default:                         /* other residues: δ = 0 */
            break;
    }
    return val;
}
