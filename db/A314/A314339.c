/*
 * A314339  Coordination sequence Gal.6.546.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + d(n) where d(n) depends on n modulo 8: n % 8 == 0:
 *            d(n) = n/2 (except n==0 -> d(0)=1) n % 8 == 1: d(n) = 1 n % 8 ==
 *            2: d(n) = -1 n % 8 == 3: d(n) = (n-3)/2 + 2 n % 8 == 4: d(n) = 0
 *            n % 8 == 5: d(n) = (n-5)/4 n % 8 == 6: d(n) = (n-6)/4 + 3 n % 8
 *            == 7: d(n) = -1 This reproduces the OEI...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t val = (seq_t)4 * (seq_t)n;          /* base term 4*n */
    switch (n & 7) {                         /* n % 8 */
        case 0:  /* n ≡ 0 (mod 8) */
            val += (seq_t)(n >> 1);          /* + n/2 */
            break;
        case 1:  /* n ≡ 1 (mod 8) */
            val += (seq_t)1;
            break;
        case 2:  /* n ≡ 2 (mod 8) */
            val -= (seq_t)1;
            break;
        case 3:  /* n ≡ 3 (mod 8) */
            val += (seq_t)(((n - 3) >> 1) + 2);
            break;
        case 4:  /* n ≡ 4 (mod 8) */
            /* no change */
            break;
        case 5:  /* n ≡ 5 (mod 8) */
            val += (seq_t)((n - 5) / 4);
            break;
        case 6:  /* n ≡ 6 (mod 8) */
            val += (seq_t)(((n - 6) / 4) + 3);
            break;
        case 7:  /* n ≡ 7 (mod 8) */
            val -= (seq_t)1;
            break;
    }
    return val;
}
