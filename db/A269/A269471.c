/*
 * A269471  Number of length-7 0..n arrays with no repeated value equal to the
 *          previous repeated value.
 * https://oeis.org/A269471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 16*n^5 + 8*n^4 - n^3 - n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 30, a(2) = 1206, …  (matches OEIS A269471) */

seq_t a(idx_t n)
{
    if (n == 0) {                /* not part of the offset but harmless */
        return 0;
    }
    seq_t tn  = (seq_t)n;         /* convert index to the value type */

    /* compute powers of n */
    seq_t n2 = tn * tn;           /* n^2 */
    seq_t n3 = n2 * tn;           /* n^3 */
    seq_t n4 = n3 * tn;           /* n^4 */
    seq_t n5 = n4 * tn;           /* n^5 */
    seq_t n6 = n5 * tn;           /* n^6 */
    seq_t n7 = n6 * tn;           /* n^7 */

    /* evaluate the polynomial */
    seq_t res = n7;               /* start with n^7 */
    res += 7 * n6;                /* + 7*n^6 */
    res += 16 * n5;               /* + 16*n^5 */
    res += 8 * n4;                /* + 8*n^4  */
    res -= n3;                    /* – n^3    */
    res -= tn;                    /* – n      */

    return res;
}
