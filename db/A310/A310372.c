/*
 * A310372  Coordination sequence Gal.4.52.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=10. For n>=3: let k = n-2. Let q = k / 7, r
 *            = k % 7. a(n) = 10 + 32*q + {0,4,8,12,18,22,26}[r] (r=0..6)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 10;

    idx_t k = n - 2;
    idx_t q = k / 7;
    idx_t r = k % 7;

    seq_t result = 10;
    result += (seq_t)q * 32;  /* 32*q fits in seq_t for all n where a fits */

    switch (r) {
        case 1: result += 4;  break;
        case 2: result += 8;  break;
        case 3: result += 12; break;
        case 4: result += 18; break;
        case 5: result += 22; break;
        case 6: result += 26; break;
        default: /* r==0 */ break;
    }
    return result;
}
