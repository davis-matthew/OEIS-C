/*
 * A260181  Numbers whose last digit is prime.
 * https://oeis.org/A260181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((n-1)/4)*10 + {2,3,5,7}[(n-1)%4]
 * Range:     u8 n <= 103, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the block index and position within the block */
    idx_t k = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    /* The four allowed last digits */
    static const seq_t last_digits[4] = {2, 3, 5, 7};

    /* Result is the base of the block plus the appropriate last digit */
    return (seq_t)k * 10 + last_digits[r];
}
