/*
 * A305930  Number of digits '0' in 3^n (in base 10).
 * https://oeis.org/A305930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit '0' in the decimal representation of 3^n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* 3^0 = 1 has no zeros */

    /* Approximate number of decimal digits of 3^n: ⌊n·log10(3)⌋+1.
       log10(3) ≈ 0.47712125472, so a safe upper bound is n*0.5+10. */
    size_t capacity = (size_t)(n * 0x2FEA / 0x14000) + 10;  /* multiply by 0.5 using fixed point */
    if (capacity < 10)                /* for very small n */
        capacity = 10;

    int *digits = (int *)malloc(capacity * sizeof(int));
    if (!digits)
        return (seq_t)0;              /* on allocation failure, return 0 */

    size_t len = 1;                   /* current number of digits, least‑significant first */
    digits[0] = 1;                    /* start from 3^0 = 1 */

    idx_t i;
    for (i = 0; i < n; ++i) {
        int carry = 0;
        size_t j;
        for (j = 0; j < len; ++j) {
            int prod = digits[j] * 3 + carry;
            digits[j] = prod % 10;
            carry    = prod / 10;
        }
        while (carry) {
            digits[len++] = carry % 10;
            carry /= 10;
            if (len == capacity) {            /* resize if needed */
                capacity <<= 1;
                digits = (int *)realloc(digits, capacity * sizeof(int));
                if (!digits)
                    return (seq_t)0;
            }
        }
    }

    seq_t zero_count = 0;
    for (size_t j = 0; j < len; ++j)
        if (digits[j] == 0)
            zero_count++;

    free(digits);
    return zero_count;
}
