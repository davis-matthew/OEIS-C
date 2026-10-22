/*
 * A325693  "Sloping septenary numbers": write numbers in septenary under each
 *          other (right-justified), read diagonals in upward direction,
 *          convert to decimal.
 * https://oeis.org/A325693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal value of the base‑7 number obtained by collecting
 *            the digits on the anti‑diagonal with index n (row – col = n)
 *            from the table of base‑7 representations, reading that diagonal
 *            from bottom to top
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Each diagonal contains at most one digit from each row m = n + j,
       namely the digit in column j.  The digit is (m / 7^j) % 7. */
    seq_t digits[64];          /* j will never exceed 62 for 64‑bit n */
    idx_t  last = 0;           /* index of last stored digit */
    seq_t  pow7 = 1;           /* 7^j */
    idx_t  j = 0;

    for (;;) {
        seq_t m = (seq_t)n + j;          /* row number */
        if (j == 0 || m >= pow7) {       /* digit exists */
            seq_t dig = (m / pow7) % 7;  /* value of the digit */
            digits[j] = dig;
            last = j;
        }

        /* If current 7^j already exceeds m (and j > 0), no further
           j will satisfy m ≥ 7^j, so the loop terminates. */
        if (pow7 > m && j > 0)
            break;

        pow7 *= 7;          /* next power of 7 */
        ++j;
    }

    /* Build the base‑7 number from the most significant digit
       (stored in digits[last]) down to the least significant one. */
    seq_t result = 0;
    for (idx_t k = last; ; --k) {
        result = result * 7 + digits[k];
        if (k == 0)
            break;
    }
    return result;
}
