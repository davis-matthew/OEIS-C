/*
 * A243208  Number of inequivalent (mod D_3) ways to place 3 points on a
 *          triangular grid of side n so that they are not vertices of an
 *          equilateral triangle with sides parallel to the grid.
 * https://oeis.org/A243208
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 3*n^5 - 3*n^4 - 2*n^3 - 48*n^2 + 48*n)/288 + IF(n
 *            odd) * (3*n^2 - 9*n - 1)/32 + IF(n%3==1) * 2/9 The denominators
 *            288, 32, 9 have LCM 288, so the whole expression can be computed
 *            using integer arithmetic with a single division by 288
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* values below the offset are ignored */

    /* cast the index to seq_t once for repeated use */
    seq_t n_ts = n;

    /* compute powers of n (they fit into seq_t by construction) */
    seq_t n2 = n_ts * n_ts;            /* n^2 */
    seq_t n3 = n2 * n_ts;              /* n^3 */
    seq_t n4 = n3 * n_ts;              /* n^4 */
    seq_t n5 = n4 * n_ts;              /* n^5 */
    seq_t n6 = n5 * n_ts;              /* n^6 */

    /* first part with denominator 288 */
    seq_t term = n6
               + (seq_t)3 * n5
               - (seq_t)3 * n4
               - (seq_t)2 * n3
               - (seq_t)48 * n2
               + (seq_t)48 * n_ts;

    /* second part (n odd) – multiply the numerator by 9 to bring it
       to denominator 288.  For n==3 the intermediate value is negative,
       so we treat that case separately to avoid unsigned underflow. */
    if (n & 1) {
        if (n == 3) {
            term -= (seq_t)9;           /* -1 * 9  = -9  */
        } else {
            seq_t add = ((seq_t)3 * n2
                         - (seq_t)9 * n_ts
                         - (seq_t)1) * (seq_t)9;
            term += add;
        }
    }

    /* third part – if n ≡ 1 (mod 3) add 64 / 288  ( = 2/9 ) */
    if (n % 3 == 1) {
        term += (seq_t)64;
    }

    /* final integer division by 288 */
    return term / (seq_t)288;
}
