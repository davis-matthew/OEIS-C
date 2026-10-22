/*
 * A307629  Let decimal expansion of n be d_1 d_2 ... d_k; a(n) =
 *          Sum_{i=1..k-1} Sum_{j=i+1..k} (d_i + d_j).
 * https://oeis.org/A307629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = (digits(n)-1) * sum_of_digits(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum over all unordered pairs of distinct decimal digits of n of (d_i + d_j). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count digits and sum them. */
    uint8_t digits = 0;          /* number of decimal digits (fits in a byte) */
    seq_t sum = 0;               /* sum of the decimal digits */

    /* Handle n = 0 explicitly; otherwise loop will not execute for n = 0 */
    if (n == 0) {
        digits = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            sum += tmp % 10;     /* add least‑significant digit */
            tmp /= 10;
            ++digits;
        }
    }

    /* If only one digit, the sum over pairs is zero. */
    if (digits <= 1)
        return 0;

    /* Compute (digits-1) * sum_of_digits in seq_t. */
    return (seq_t)(digits - 1) * sum;
}
