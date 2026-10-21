/*
 * A274324  Number of partitions of n^3 into at most two parts.
 * https://oeis.org/A274324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3+(-1)^n+2*n^3)/4
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^3. */
    seq_t n3 = (seq_t)n;
    n3 *= (seq_t)n;
    n3 *= (seq_t)n;

    /* Compute numerator: 3 + 2*n3 plus or minus 1 depending on parity of n. */
    seq_t num = 3 + 2 * n3;

    if ((n & 1) == 0) {   /* n is even: add 1 */
        num += 1;
    } else {              /* n is odd: subtract 1 */
        num -= 1;
    }

    return num / 4;
}
