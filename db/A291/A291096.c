/*
 * A291096  Number of rooted gluings of octahedra with n square vertices.
 * https://oeis.org/A291096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * (4n)! / (n! (3n+1)!) * This follows from the
 *            generating function A(x) = 1 + 3xA(x)^4 * using Lagrange
 *            inversion: a(n) = 3^n * binomial(4n+1, n) / (4n+1) * = 3^n *
 *            (4n)! / (n! (3n+1)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is 0, the formula gives 1 directly */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t res = (seq_t)1;            /* will hold binomial(4n, n) */
    seq_t nseq = (seq_t)n;           /* integer type compatible with seq_t */
    seq_t four_n = (seq_t)4 * nseq;  /* 4 * n */

    /* Compute binomial(4n, n) online: binom = 1; for k=1..n: binom = binom * (4n - k + 1) / k */
    for (idx_t k = 1; k <= n; ++k) {
        res = res * (four_n - (seq_t)k + (seq_t)1) / (seq_t)k;
    }

    /* Divide by (3n + 1).  This division is exact. */
    res = res / ((seq_t)3 * nseq + (seq_t)1);

    /* Compute 3^n */
    seq_t pow3 = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        pow3 *= (seq_t)3;
    }

    /* Final result: res * 3^n */
    return res * pow3;
}
