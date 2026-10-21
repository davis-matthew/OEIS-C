/*
 * A289592  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 187) or the same sequence
 *          for the mesh pattern (12, 442).
 * https://oeis.org/A289592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations avoiding pattern 321 and the mesh
 *            * pattern (12,187). The sequence satisfies * * a(0) = a(1) =
 *            a(2) = 1 * for n ≥ 3 * a(n) = C_n - 2·C_{n-1} + 2·C_{n-2} -
 *            C_{n-3} * * where C_k are the Catalan numbers. Catalan numbers
 *            are generated * iteratively using * C_{k+1} = C_k...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    /* buffer for the last four Catalan numbers */
    seq_t c[4];
    c[0] = (seq_t)1;   /* C0 */

    /* compute C_1 … C_n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult  = (seq_t)2 * (seq_t)(2 * (i - 1) + 1);  /* 2*(2*(i‑1)+1) */
        seq_t denom = (seq_t)(i + 1);                       /* i+1          */
        c[i & 3] = c[(i - 1) & 3] * mult / denom;          /* exact division */
    }

    /* indices of required Catalan numbers */
    idx_t i_n  = n & 3;
    idx_t i_n1 = (n - 1) & 3;
    idx_t i_n2 = (n - 2) & 3;
    idx_t i_n3 = (n - 3) & 3;

    /* a(n) = C_n + 2*C_{n-2}  –  (2*C_{n-1} + C_{n-3}) */
    seq_t pos = c[i_n] + c[i_n2];
    pos += c[i_n2];                     /* add another C_{n-2} */
    seq_t neg = c[i_n1] + c[i_n1];
    neg += c[i_n3];
    seq_t result = pos - neg;            /* result is non‑negative for n≥3 */

    return result;
}
