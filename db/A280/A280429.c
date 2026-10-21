/*
 * A280429  Longest word T from a string S using no breakpoint-reuse.
 * https://oeis.org/A280429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^k)*(n-5)+2*k+5 with k = floor((n-1)/3) (Special
 *            handling for n = 1..4 to avoid negative intermediate values.)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly to avoid negative products */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    if (n == 4) return 5;

    /* Compute k = floor((n-1)/3) */
    idx_t k = (n - 1) / 3;

    /* Compute 2^k */
    seq_t pow2k = 1;
    for (idx_t i = 0; i < k; ++i)
        pow2k *= 2;

    /* Compute the formula */
    seq_t term = pow2k * ((seq_t)n - 5);
    term += ((seq_t)2 * k) + 5;
    return term;
}
