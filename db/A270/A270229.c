/*
 * A270229  Number of matchings in the 2 X n rook graph P_2 X K_n.
 * https://oeis.org/A270229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) + (n-1)^2*a(n-2) - (n-2)*(n-1)^2*a(n-3) +
 *            (n-3)*(n-2)*(n-1)*a(n-4) Initial values: a(0)=1, a(1)=2, a(2)=7,
 *            a(3)=32
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n: return the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)32;

    /* Maintain the last four values: a(k‑4), a(k‑3), a(k‑2), a(k‑1) */
    seq_t a0 = (seq_t)1;   /* a(k-4) when k == 4 */
    seq_t a1 = (seq_t)2;   /* a(k-3) */
    seq_t a2 = (seq_t)7;   /* a(k-2) */
    seq_t a3 = (seq_t)32;  /* a(k-1) */

    for (idx_t k = 4; k <= n; ++k) {
        /* Compute a(k) using the recurrence */
        seq_t t1 = (seq_t)(k + 1) * a3;
        seq_t t2 = (seq_t)(k - 1) * (seq_t)(k - 1) * a2;
        seq_t t3 = (seq_t)(k - 2) * (seq_t)(k - 1) * (seq_t)(k - 1) * a1;
        seq_t t4 = (seq_t)(k - 3) * (seq_t)(k - 2) * (seq_t)(k - 1) * a0;

        seq_t ak = t1 + t2;
        ak -= t3;
        ak += t4;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ak;
    }
    return a3;  /* a(n) is stored in a3 after the loop */
}
