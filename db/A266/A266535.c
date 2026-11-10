/*
 * A266535  Sums of two successive terms of A256249, with a(0) = 0.
 * https://oeis.org/A266535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A256249(n-1) + A256249(n) for n >= 1, a(0) = 0, where
 *            A256249(n) = n*(n+2) - (2*(4^L-1)/3 + 2^(L+1)*(n-2^L+1)), L =
 *            floor(log2 n)
 * Range:     u8 n <= 15, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9378 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A256249_a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t L = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++L;

    seq_t pow4   = ((seq_t)1) << (2 * L);
    seq_t pow2L  = ((seq_t)1) << L;
    seq_t pow2L1 = ((seq_t)1) << (L + 1);

    seq_t P = ((seq_t)2 * (pow4 - (seq_t)1)) / (seq_t)3;
    P += pow2L1 * ((((seq_t)n) - pow2L) + (seq_t)1);

    seq_t res = ((seq_t)n) * ((((seq_t)n) + (seq_t)2)) - P;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    return A256249_a(n - 1) + A256249_a(n);
}
