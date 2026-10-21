/*
 * A284804  Fixed points of the transform A284803.
 * https://oeis.org/A284804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer whose base‑6 representation * satisfies
 *            d_i + d_{2k+1-i} = 5 for all positions (i = 1 … 2k)
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th term of OEIS sequence A284804.
 *  The algorithm enumerates numbers whose base‑6 digits come in complementary
 *  pairs summing to 5.  It finds the length of the number by iterating
 *  over powers of 6, determines the offset within that length, builds the
 *  left half of the digits, assembles the full digit string, and converts
 *  it back to base‑10. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0, return 0 */

    /* Find the smallest k such that 6^k - 1 >= n.
     * 6^k is stored in pow6 */
    seq_t pow6 = 1;
    idx_t  k    = 0;
    while ((pow6 - 1) < n) {
        pow6 = pow6 * (seq_t)6;
        ++k;
    }

    /* pow6 = 6^k;  prev_pow6 = 6^(k-1)  */
    seq_t prev_pow6 = pow6 / (seq_t)6;          /* 6^(k-1)  */
    seq_t t_seq    = (seq_t)n - prev_pow6;      /* offset within current length */

    seq_t power = prev_pow6;    /* 6^(k-1)   */
    seq_t first_offset = t_seq / power;         /* 0 .. 4   */
    seq_t first_digit  = first_offset + (seq_t)1;/* 1 .. 5   */
    seq_t rem          = t_seq - first_offset * power;

    /* left half of the digits (k digits)  */
    unsigned char left[64];                     /* k <= 40 for n up to 2^64-1 */
    left[0] = (unsigned char)first_digit;

    for (idx_t i = 1; i < k; ++i) {
        power = power / (seq_t)6;                 /* 6^(k-1-i) */
        seq_t digit = rem / power;               /* 0 .. 5   */
        left[i] = (unsigned char)digit;
        rem   = rem % power;
    }

    /* Assemble the full base‑6 digit string and convert to decimal */
    seq_t val = 0;
    for (idx_t j = 0; j < 2 * k; ++j) {
        seq_t d;
        if (j < k)
            d = (seq_t)left[j];
        else
            d = (seq_t)5 - (seq_t)left[2 * k - 1 - j];
        val = val * (seq_t)6 + d;
    }
    return val;
}
