/*
 * A284971  Numbers with digits 4 and 7 only.
 * https://oeis.org/A284971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest positive integer consisting only of
 *            decimal digits 4 and 7, ordered increasingly. The value is
 *            obtained by interpreting the binary value of (n‑1) as digits 4
 *            (0) and 7 (1), padded to the minimal length so that the
 *            cumulative count of shorter numbers is less than n
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the length L of the smallest number of digits that can
       contain the n‑th term.  L = floor(log2(n+1)). */
    idx_t t = n + 1;           /* (n+1) is > 1 for n >= 1 */
    idx_t L = 0;
    while (t > 1) {
        t >>= 1;
        L++;
    }

    /* Count of all numbers whose length is less than L:
       total_prev = 2^L - 2  (since 2^1 + 2^2 + ... + 2^{L-1} = 2^L - 2) */
    idx_t total_prev = ((idx_t)1 << L) - 2;
    /* Zero‑based offset of the desired number within the block of
       L‑digit numbers. */
    idx_t offset = n - total_prev - 1;

    seq_t value = 0;
    /* Build the digits from most significant to least significant. */
    for (idx_t i = L; i > 0; --i) {
        seq_t digit = ((offset >> (i - 1)) & 1) ? 7 : 4;
        value = value * 10 + digit;
    }
    return value;
}
