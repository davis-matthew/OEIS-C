/*
 * A384136  a(n) = (3*n)!/(2*n)! * Sum_{k=1..n} 1/(2*n+k).
 * https://oeis.org/A384136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n)!/(2*n)! * Sum_{k=1..n} 1/(2*n+k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is not defined for the OEIS offset but return 0 for safety. */
    if (n == 0) return 0;

    /* Compute (3n)!/(2n)! = product_{i=2n+1}^{3n} i. */
    seq_t fact = 1;
    idx_t start = 2 * n + 1;
    idx_t end   = 3 * n;
    for (idx_t i = start; i <= end; ++i) {
        fact *= (seq_t)i;
    }

    /* Sum the terms fact/(2n+k) for k=1..n (inclusive). */
    seq_t sum = 0;
    idx_t denom_start = 2 * n + 1;
    for (idx_t k = 0; k < n; ++k) {
        sum += fact / ((seq_t)(denom_start + k));
    }

    return sum;
}
