/*
 * A284811  Fixed points of the transform A267193.
 * https://oeis.org/A284811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth antipalindromic number in base 10 (digits appear
 *            in complementary pairs summing to 9)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a given n ≥ 1, the number has an even length 2k, where k = ⌈log10(n+1)⌉.  
   It is constructed as a = A · 10^k + B, where A is the k‑digit integer
   A = 10^(k‑1) + (n – (10^(k‑1) – 1) – 1), and B consists of the complementary
   digits of A in reverse order. */

seq_t a(idx_t n)
{
    /* Determine k such that 10^k ≥ n+1. */
    seq_t n_seq = (seq_t)n;
    idx_t k = 1;
    seq_t pow10k = 10;
    while (n_seq + 1 > pow10k) {
        ++k;
        pow10k *= 10;
    }

    /* pow10k = 10^k, pow10_kminus1 = 10^(k‑1). */
    seq_t pow10_kminus1 = pow10k / 10;

    /* Index within the block of length 2k numbers. */
    seq_t prev  = pow10_kminus1 - 1;          /* total numbers before length 2k */
    seq_t m     = n_seq - prev;                /* 1‑based position in this block */

    /* Construct the first half A. */
    seq_t A = pow10_kminus1 + (m - 1);

    /* Build the second half B as the reversed complement of A. */
    seq_t temp = A;
    seq_t B = 0;
    for (idx_t i = 0; i < k; ++i) {
        seq_t digit = temp % 10;
        temp /= 10;
        seq_t comp = 9 - digit;
        B = B * 10 + comp;
    }

    /* Combine to get the full antipalindromic number. */
    return A * pow10k + B;
}
