/*
 * A375976  Sum of squares of the decimal digits of 2^n.
 * https://oeis.org/A375976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of squares of the decimal digits of 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_digits, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    seq_t pow_val = pow2(n);           /* 2^n as a seq_t */

    /* Estimate a buffer size: 2^n has about n/3.32 decimal digits.
       Allocate a few extra to be safe. */
    size_t cap = (size_t)(n / 3 + 20);
    if (cap < 16)
        cap = 16;                      /* minimal room for tiny n */

    uint8_t *buf = (uint8_t *)malloc(cap);
    if (!buf)
        return 0;                      /* out‑of‑memory fallback */

    size_t len = 0;
    get_digits(pow_val, buf, &len);    /* decimal digits of 2^n */

    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += (seq_t)(buf[i] * buf[i]); /* add square of each digit */

    free(buf);
    return sum;
}
