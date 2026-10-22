/*
 * A315872  Coordination sequence Gal.5.252.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=6; a(2)=8; For n≥3: n%4==0 → a(n)=22*(n/4) n%4==1 →
 *            a(n)=6+20*((n-1)/4) n%4==2 → a(n)=10+16*((n-2)/4) n%4==3 →
 *            a(n)=12+20*((n-3)/4)
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)8;

    idx_t r = n & 3; /* n % 4 */
    switch (r) {
        case 0: {
            /* n = 4k */
            idx_t k = n / 4;
            return (seq_t)(22 * k);
        }
        case 1: {
            /* n = 4k+1 */
            idx_t k = (n - 1) / 4;
            return (seq_t)(6 + 20 * k);
        }
        case 2: {
            /* n = 4k+2, k>=1 (n≥6) */
            if (n == 2) return (seq_t)8; /* handled above but kept for safety */
            idx_t k = (n - 2) / 4;
            return (seq_t)(10 + 16 * k);
        }
        case 3: {
            /* n = 4k+3 */
            idx_t k = (n - 3) / 4;
            return (seq_t)(12 + 20 * k);
        }
    }
    return (seq_t)0; /* unreachable */
}
