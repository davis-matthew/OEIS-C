/*
 * A284632  Numbers n with digits 2 and 6 only.
 * https://oeis.org/A284632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * A032917(n)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* below offset – value irrelevant */

    /* Determine the number of digits k: k = floor(log2(n+1)) */
    idx_t k   = 0;
    idx_t tmp = n + 1;
    while (tmp > 1) {
        tmp >>= 1;
        ++k;
    }

    /* Power of two used to locate the group for this n */
    idx_t pow2 = ((idx_t)1) << k;

    /* 0‑based index inside the current digit‑length group */
    idx_t r = n - pow2 + 1;

    /* Construct the decimal number from the binary pattern of r */
    seq_t result = 0;
    for (idx_t i = k; i > 0; --i) {
        idx_t bit   = (r >> (i - 1)) & 1;
        seq_t digit = bit ? (seq_t)6 : (seq_t)2;
        result = result * 10 + digit;
    }

    return result;
}
