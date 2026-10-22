/*
 * A382170  a(0) = 0; for n >= 1, one-eighth of the number of points on the
 *          elliptic curve y^2 = x^3 - x defined over GF(5^n).
 * https://oeis.org/A382170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, and for n ≥ 2 * a(n) = 5^{n‑1} + 1 –
 *            2·a(n‑1) – 5·a(n‑2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)0;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t pow5 = (seq_t)1;  /* 5^0 */

    for (idx_t k = 2; k <= n; ++k) {
        pow5 *= (seq_t)5;                         /* now 5^{k‑1}   */
        seq_t anum = pow5 + (seq_t)1              /* 5^{k‑1}+1     */
                     - (seq_t)2 * a1                 /* – 2 a(k‑1)    */
                     - (seq_t)5 * a0;                /* – 5 a(k‑2)    */
        a0 = a1;   /* shift the window   */
        a1 = anum;
    }
    return a1;
}
