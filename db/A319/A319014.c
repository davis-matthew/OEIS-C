/*
 * A319014  a(n) = 1*2*3 + 4*5*6 + 7*8*9 + 10*11*12 + 13*14*15 + 16*17*18 +
 *          ... + (up to n).
 * https://oeis.org/A319014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*k) = 3*k*(k+1)*(3*k-2)*(3*k+1)/4 a(3*k+1) = a(3*k) + (3*k+1)
 *            a(3*k+2) = a(3*k) + (3*k+2)*(3*k+1) Offset: 1
 * Range:     u8 n <= 8, u16 n <= 29, u32 n <= 476, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute k = floor(n/3) */
    idx_t k = n / 3;
    seq_t a3k;

    if (k == 0) {
        /* for n = 1 or 2 */
        a3k = 0;
    } else {
        /* compute product 3*k*(k+1)*(3*k-2)*(3*k+1) / 4
           divide one of the factors by 4 first to avoid overflow */
        if ((3 * k) % 4 == 0) {
            a3k = (seq_t)(3 * k / 4) * (seq_t)(k + 1) *
                  (seq_t)(3 * k - 2) * (seq_t)(3 * k + 1);
        } else if ((k + 1) % 4 == 0) {
            a3k = (seq_t)(3 * k) * (seq_t)((k + 1) / 4) *
                  (seq_t)(3 * k - 2) * (seq_t)(3 * k + 1);
        } else if ((3 * k - 2) % 4 == 0) {
            a3k = (seq_t)(3 * k) * (seq_t)(k + 1) *
                  (seq_t)((3 * k - 2) / 4) * (seq_t)(3 * k + 1);
        } else {
            /* then 3*k+1 is guaranteed divisible by 4 */
            a3k = (seq_t)(3 * k) * (seq_t)(k + 1) *
                  (seq_t)(3 * k - 2) * (seq_t)((3 * k + 1) / 4);
        }
    }

    idx_t r = n % 3;
    if (r == 0) {
        return a3k;
    } else if (r == 1) {
        return a3k + (seq_t)(3 * k + 1);
    } else { /* r == 2 */
        return a3k + (seq_t)(3 * k + 2) * (seq_t)(3 * k + 1);
    }
}
