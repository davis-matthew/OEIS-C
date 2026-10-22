/*
 * A315965  Coordination sequence Gal.5.254.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9*n + r(n)) / 2 where r(n) = { 2 (n = 0) 2*(n/6) (n % 6
 *            = 0, n>0) 3 (n % 6 = 1) -(n-2)/3 (n % 6 = 2) -1 (n % 6 = 3) 0 (n
 *            % 6 = 4) -3 (n % 6 = 5) }
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n % 6;
    int64_t r;
    if (k == 0) {
        if (n == 0)
            r = 2;
        else
            r = 2 * (n / 6);
    } else if (k == 1) {
        r = 3;
    } else if (k == 2) {
        r = - (int64_t)((n - 2) / 3);
    } else if (k == 3) {
        r = -1;
    } else if (k == 4) {
        r = 0;
    } else { /* k == 5 */
        r = -3;
    }

    seq_t num = (seq_t)n * 9;
    if (r >= 0)
        num += (seq_t)r;
    else
        num -= (seq_t)(-r);

    return num / 2;
}
