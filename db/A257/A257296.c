/*
 * A257296  Arithmetic mean of the digits of n, multiplied by 10^(d-1) and
 *          rounded down, where d is the number of digits of n.
 * https://oeis.org/A257296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = ⌊ (A007953(n)/A055642(n))*10^(A055642(n)-1) ⌋ = ⌊
 *            (sum_of_digits(n) * 10^(d-1)) / d ⌋ where d = digit_count(n)
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Explicitly handle the offset n = 0. */
    if (n == 0)
        return 0;

    seq_t sum = 0;  /* sum of decimal digits */
    int    d   = 0; /* number of digits */
    idx_t  tmp = n;

    /* Count digits and compute digit sum. */
    while (tmp > 0) {
        sum += (seq_t)(tmp % 10);
        tmp /= 10;
        ++d;
    }

    /* 10^(d-1) as a seq_t value. */
    seq_t pow10x = pow10((idx_t)(d - 1));

    /* Numerator: sum * 10^(d-1) */
    seq_t numerator = sum * pow10x;

    /* Integer division gives the floor. */
    return numerator / (seq_t)d;
}
