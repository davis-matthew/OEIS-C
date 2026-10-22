/*
 * A317952  Trajectory of 1 under repeated application of the morphism 1->121,
 *          2->232, 3->343, 4->414.
 * https://oeis.org/A317952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3*k) = a(3*k+2) = a(k); a(3*k+1) == (1 + a(k)) mod 4 with 0
 *            replaced by 4. a(0) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t q = n / 3;
    idx_t r = n % 3;
    seq_t v = a(q);

    if (r == 1) {
        seq_t tmp = (v % 4) + 1;     /* (1 + a(k)) mod 4 */
        seq_t res = tmp % 4;
        if (res == 0) res = 4;       /* replace 0 with 4 */
        return res;
    } else {
        return v;                    /* a(3*k) or a(3*k+2) */
    }
}
