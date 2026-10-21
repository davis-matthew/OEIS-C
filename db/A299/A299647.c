/*
 * A299647  Positive solutions to x^2 == -2 (mod 11).
 * https://oeis.org/A299647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 * floor((n-1)/2) + (n odd ? 3 : 8)
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299647: positive solutions to x^2 ≡ -2 (mod 11), i.e. numbers ≡ 3 or 8 mod 11. */
seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 2;                /* floor((n-1)/2) */
    /* Compute in seq_t to avoid overflow when seq_t is 128-bit */
    seq_t result = ((seq_t)11) * k + ((n & 1) ? (seq_t)3 : (seq_t)8);
    return result;
}
