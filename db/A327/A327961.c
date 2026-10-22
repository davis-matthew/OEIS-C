/*
 * A327961  Sum of products of n-bit numbers with their n-bit reverse.
 * https://oeis.org/A327961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^(n-3) * (2 * (2^n-1)^2 + 2^n * n) for n >= 3 (special
 *            cases: a(0)=0, a(1)=1, a(2)=13)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly to avoid negative shifts */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;

    /* compute 2^n as a shift; this is safe because n >= 3 */
    seq_t pow2n = ((seq_t)1 << n);
    seq_t pow2n_minus1 = pow2n - (seq_t)1;

    /* 2 * (2^n-1)^2 */
    seq_t part1 = ((pow2n_minus1 * pow2n_minus1) << 1);

    /* 2^n * n */
    seq_t part2 = pow2n * ((seq_t)n);

    /* inner sum */
    seq_t inner = part1 + part2;

    /* 2^(n-3) */
    seq_t pref = ((seq_t)1 << (n - 3));

    /* final result */
    return pref * inner;
}
