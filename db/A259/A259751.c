/*
 * A259751  Numbers that are congruent to {8, 16} mod 24.
 * https://oeis.org/A259751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n - 2*(-1)^n - 6 (for n ≥ 1)
 * Range:     u8 n <= 21, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1; for n == 0 return 0 to avoid underflow */
    if (n == 0) {
        return (seq_t)0;
    }

    /* compute 12*n */
    seq_t val = (seq_t)n * (seq_t)12;

    /* subtract 8 if n is even, subtract 4 if n is odd */
    if ((n & 1ULL) == 0) {
        val -= (seq_t)8;
    } else {
        val -= (seq_t)4;
    }

    return val;
}
