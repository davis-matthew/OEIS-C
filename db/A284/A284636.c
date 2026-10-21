/*
 * A284636  Numbers with digits 6 and 9 only.
 * https://oeis.org/A284636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3 * A032810(n)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer whose decimal digits are only 6 or 9. */
/* The digits are produced by interpreting (n+1) in binary (dropping the leading 1), */
/* mapping 0→6 and 1→9, and concatenating them as decimal digits. */

seq_t a(idx_t n)
{
    /* n is 1‑based.  n == 0 is not part of the sequence; return 0 as guard. */
    if (n == 0)
        return 0;

    /* Find L = floor(log2(n+1)).  L is the number of digits. */
    idx_t tmp = n + 1;
    idx_t L = 0;
    while (tmp >>= 1)
        ++L;

    /* r is the offset within the block of length L (0‑based). */
    idx_t r = n + 1 - (1ULL << L);

    seq_t result = 0;
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t shift = L - 1 - pos;
        idx_t bit   = (r >> shift) & 1ULL;
        seq_t digit = 6 + 3 * bit;          /* 6 for 0, 9 for 1 */
        result = result * 10 + digit;
    }
    return result;
}
