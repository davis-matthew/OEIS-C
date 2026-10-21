/*
 * A294106  Sum of products of terms in all partitions of 3*n into distinct
 *          powers of 3.
 * https://oeis.org/A294106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of products of terms in all partitions of 3*n into
 *            distinct powers of 3
 * Range:     u8 n <= 12, u16 n <= 93, u32 n <= 363, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t product = (seq_t)1;   /* product of selected powers, start with empty product */
    seq_t power   = (seq_t)3;   /* 3^(position+1): first possible power is 3^1 */
    idx_t m = n;                /* we work on the base‑3 digits of n (since 3n has a trailing 0) */

    while (m > 0) {
        idx_t digit = m % 3;    /* current ternary digit of n (0, 1, or 2) */
        if (digit == 2)         /* digit 2 → no valid partition */
            return (seq_t)0;
        if (digit == 1)         /* digit 1 → include 3^(position+1) in product */
            product *= power;
        m /= 3;
        power *= 3;             /* move to the next power of 3 */
    }

    return product;
}
