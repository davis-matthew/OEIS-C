/*
 * A378796  Number of minimal edge cuts in the n-sun graph.
 * https://oeis.org/A378796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n + (-1)^n)/2 + n - 1 = A046717(n) + n - 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t holds A046717(n) = (3^n + (-1)^n)/2  */
    seq_t t = 1;                               /* t(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        t = t * ((seq_t)3);                    /* t = 3 * t(i‑1) */
        if ((i & 1) == 0) {                    /* i even  → add 2 */
            t += (seq_t)2;
        } else {                               /* i odd   → subtract 2 */
            t -= (seq_t)2;
        }
    }

    /* a(n) = t + n - 1 */
    return t + ((seq_t)(n - 1));
}
