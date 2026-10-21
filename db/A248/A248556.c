/*
 * A248556  Concatenate (3n-2,3n-1,3n).
 * https://oeis.org/A248556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (3*n-2)*10^(digits(3*n-1)+digits(3*n)) +
 *            (3*n-1)*10^(digits(3*n)) + 3*n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 333, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: number of decimal digits of a seq_t value */
static int _digit_len(seq_t x)
{
    if (x == 0) return 1;
    int len = 0;
    seq_t tmp = x;
    while (tmp > 0)
    {
        tmp /= 10;
        ++len;
    }
    return len;
}

/* Helper: 10^e as seq_t, e is a non‑negative idx_t */
static seq_t _pow10(idx_t e)
{
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i)
        result *= 10;
    return result;
}

seq_t a(idx_t n)
{
    /* offset is 1; a(0) is irrelevant */
    if (n == 0)
        return 0;

    /* three_n = 3*n, cast to seq_t for arithmetic */
    seq_t three_n = (seq_t)(3 * (seq_t)n);
    seq_t part1 = three_n - 2;   /* 3n-2 */
    seq_t part2 = three_n - 1;   /* 3n-1 */
    seq_t part3 = three_n;       /* 3n */

    int d2 = _digit_len(part2);
    int d3 = _digit_len(part3);

    seq_t pow10_d3 = _pow10((idx_t)d3);
    seq_t pow10_d2d3 = _pow10((idx_t)(d2 + d3));

    return part1 * pow10_d2d3 + part2 * pow10_d3 + part3;
}
