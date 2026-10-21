/*
 * A256137  a(2) = 1; a(3) = 4; for n >= 4, a(n) = 2 + Sum_{i=4..n} d(i),
 *          where d(i) = i for even i, d(i) = i-3 for odd i.
 * https://oeis.org/A256137
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256137
 * a(2) = 1, a(3) = 4, for n ≥ 4
 * a(n) = 2 + Σ_{i=4..n} d(i) where d(i)=i if i even, d(i)=i-3 if i odd.
 * Equivalently for n>3:
 *   a(n) = (5 + 3*(-1)^n - 4*n + 2*n^2) / 4
 *   a(n) = (n^2 - 2*n + 4)/2   for even n>3
 *   a(n) = (n^2 - 2*n + 1)/2   for odd  n>3
 */
seq_t a(idx_t n)
{
    if (n == 2) return 1;
    if (n == 3) return 4;

    if ((n & 1U) == 0) {          /* even n ≥ 4 */
        seq_t n_sq   = (seq_t)n * (seq_t)n;           /* n^2   */
        seq_t two_n  = (seq_t)2 * (seq_t)n;           /* 2*n   */
        return (n_sq + (seq_t)4 - two_n) / (seq_t)2;  /* (n^2+4-2n)/2 */
    } else {                       /* odd n ≥ 5 */
        seq_t n_sq   = (seq_t)n * (seq_t)n;           /* n^2   */
        seq_t two_n  = (seq_t)2 * (seq_t)n;           /* 2*n   */
        return (n_sq + (seq_t)1 - two_n) / (seq_t)2;  /* (n^2+1-2n)/2 */
    }
}
