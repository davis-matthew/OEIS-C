/*
 * A378834  Number of ways to reverse a permutation of n elements by the
 *          minimum number of steps of rotate left or right 1 place (L,R), or
 *          exchange first two elements (E).
 * https://oeis.org/A378834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 for n = 1 a(n) = 3 for n = 2 a(n) = 2 for n = 3 For n ≥
 *            4, let h = ⌊(n‑4)/2⌋. a(n) = (h + 2) · 2^h · h! (which yields
 *            the same value for consecutive n = 2k and 2k+1 (k ≥ 3), e.g.
 *            a(6)=a(7)=6, a(8)=a(9)=32, …) The implementation computes the
 *            factorial and power of two by repeated multipli...
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)2;

    /* For n >= 4, compute h = floor((n-4)/2). */
    idx_t h = (n - 4) / 2;

    /* Compute 2^h. */
    seq_t two_pow = (seq_t)1;
    for (idx_t i = 0; i < h; ++i) {
        two_pow *= (seq_t)2;
    }

    /* Compute h! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= h; ++i) {
        fact *= (seq_t)i;
    }

    /* Assemble the final result. */
    seq_t result = ((seq_t)(h + 2)) * two_pow;
    result *= fact;
    return result;
}
