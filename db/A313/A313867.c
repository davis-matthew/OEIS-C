/*
 * A313867  Coordination sequence Gal.6.577.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊11 n / 2⌋ – c(n) where c(n) is defined as: n = 0 → a(0)
 *            = 1 n > 0, even → c(n) = 1 n > 0, odd: n % 4 == 1: if n/4 == 0 →
 *            c = 0 (n = 1) if n/4 == 1 → c = 2 (n = 5,7) else → c = 2*(n/4) –
 *            1 n % 4 == 3: c = 2*(n/4)
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* floor(11*n / 2) */
    seq_t tri = ((seq_t)n * 11) / 2;

    seq_t c;
    if ((n & 1) == 0) {           /* even n > 0 */
        c = 1;
    } else {                      /* odd n > 0 */
        idx_t q = n / 4;          /* floor(n/4) */
        if ((n & 3) == 1) {       /* n % 4 == 1 */
            if (q <= 1) {
                /* n = 1 → 0, n = 5 or 7 (q==1) → 2 */
                c = (q == 0) ? 0 : 2;
            } else {
                /* n >= 9 with n%4==1 */
                c = (seq_t)(2 * q - 1);
            }
        } else {                  /* n % 4 == 3 */
            /* n = 3 → 0, otherwise 2*q */
            c = (seq_t)(2 * q);
        }
    }

    return tri - c;
}
