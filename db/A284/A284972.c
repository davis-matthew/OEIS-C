/*
 * A284972  Numbers with digits 4 and 8 only.
 * https://oeis.org/A284972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th number consisting of only the decimal digits 4
 *            and 8 (in increasing order)
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0‑based index from n */
    idx_t m = n - 1;

    /* Find the length L of the digit string.  The total count of
       numbers with up to length L is C(L) = 2 + 4 + ... + 2^L
       = 2*(2^L - 1).  We find the smallest L with C(L) > m. */
    idx_t total = 0;        /* numbers counted so far */
    idx_t pow    = 2;       /* 2^1 */
    idx_t L      = 1;       /* current length */

    while (total + pow <= m) {
        total += pow;     /* include all numbers of this length */
        pow <<= 1;        /* next power of two (2^(L+1)) */
        L++;
    }

    /* Offset within the block of length L numbers */
    idx_t offset = m - total;   /* 0 <= offset < 2^L */

    /* Convert offset to a decimal number with digits 4 (bit 0)
       and 8 (bit 1).  The most significant bit corresponds to
       the most significant decimal digit. */
    seq_t value = 0;
    for (idx_t i = L; i-- > 0;) {
        uint64_t bit = (offset >> i) & 1;
        seq_t digit = 4 + 4 * bit;  /* 4 or 8 */
        value = value * 10 + digit;
    }
    return value;
}
