/*
 * A264100  Sum of the lengths of the arithmetic progressions in
 *          {1,2,3,...,n}, including trivial arithmetic progressions of
 *          lengths 1 and 2.
 * https://oeis.org/A264100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * + Sum_{k=2..n} k*floor((n-1)/(k-1))*
 *            (2*n-(k-1)*floor((n+k-2)/(k-1)))/2 * This implementation uses
 *            the direct summation over k, which is O(n) and fast * enough for
 *            n up to at least 10000
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t total = nseq;   /* trivial length‑1 progressions */

    if (n < 2)
        return total;

    for (idx_t k = 2; k <= n; ++k) {
        idx_t denom = k - 1;
        idx_t M     = (n - 1) / denom;        /* floor((n-1)/(k-1)) */
        seq_t kseq  = (seq_t)k;
        seq_t Mseq  = (seq_t)M;
        seq_t denomseq = (seq_t)denom;
        seq_t A    = (seq_t)(M + 1);           /* M + 1 */

        /* T = 2*n - denom*A  (all in seq_t) */
        seq_t T = (seq_t)2 * nseq - denomseq * A;

        /* term = k * M * T / 2 */
        seq_t term = kseq * Mseq * T / 2;

        total += term;
    }
    return total;
}
