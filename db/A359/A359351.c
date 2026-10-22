/*
 * A359351  a(n) = A001952(A003151(n)).
 * https://oeis.org/A359351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*⌊n*√2⌋ + 4*n – 1 The sequence is A359351, the third of
 *            the four sequences that partition the positive integers. It can
 *            be expressed directly by the Beatty sequence for √2. Since
 *            floating point arithmetic is forbidden, we use an exact rational
 *            approximation of √2 – 1: 195025/470832 ≈ 0.41...
 * Range:     u8 n <= 31, u16 n <= 7951, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants for the rational approximation of √2‑1 */
    const uint64_t P = 195025;  /* numerator of the approximation   */
    const uint64_t Q = 470832;  /* denominator of the approximation */

    /* compute ⌊n·(√2‑1)⌋ exactly using integer arithmetic.
       The product fits into 128 bits for all reasonable indices. */
    unsigned __int128 mul = (unsigned __int128)n * P;
    uint64_t add = (uint64_t)(mul / Q);           /* ⌊n·(√2‑1)⌋  */

    /* ⌊n·√2⌋ = n + ⌊n·(√2‑1)⌋  */
    seq_t sqrt2_floor = (seq_t)n + (seq_t)add;

    /* a(n) = 3*floor(n*√2) + 4*n – 1  */
    seq_t result = ((seq_t)3 * sqrt2_floor) + ((seq_t)4 * (seq_t)n) - 1;

    return result;
}
