/*
 * A269586  Number of length-6 0..n arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 9*n^4 + 22*n^3 + 15*n^2 + 12*n - 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* As n >= 1 per OEIS offset, computing powers of n and applying the
       closed‑form polynomial works in O(1) time and is exact for all n
       whose result fits in seq_t. */

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */
    seq_t n5 = n4 * nn;           /* n^5 */
    seq_t n6 = n5 * nn;           /* n^6 */

    seq_t ans = n6;
    ans += (seq_t)6 * n5;
    ans += (seq_t)9 * n4;
    ans += (seq_t)22 * n3;
    ans += (seq_t)15 * n2;
    ans += (seq_t)12 * nn;
    ans -= (seq_t)1;

    return ans;
}
