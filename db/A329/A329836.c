/*
 * A329836  Beatty sequence for (11+sqrt(101))/10.
 * https://oeis.org/A329836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(11 + sqrt(101))/10)
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1, undefined for n=0 */

    seq_t n_t = (seq_t)n;                /* cast index to seq_t */
    /* k = floor(n*sqrt(101)) = floor(sqrt(101*n^2))                */
    seq_t val = n_t * n_t;               /* n^2 */
    val *= 101;                          /* 101 * n^2 */

    seq_t low  = 0;
    seq_t high = n_t * 11;               /* sqrt(101) < 11, so 11*n is an upper bound */

    while (low <= high) {
        seq_t mid  = (low + high) / 2;
        seq_t mid2 = mid * mid;

        if (mid2 <= val) {
            low = mid + 1;
        } else {
            /* mid2 > val, avoid underflow when mid == 0 */
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    seq_t k = high;                      /* floor sqrt */

    /* compute floor((11*n + k)/10) */
    seq_t numerator = (seq_t)11 * n_t + k;
    return numerator / 10;
}
