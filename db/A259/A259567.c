/*
 * A259567  Number of subsequent numbers, starting with n, for which
 *          A258881(x) = x + (sum of squares of digits of x) is prime.
 * https://oeis.org/A259567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of consecutive integers starting with n for which
 *            f(x) = x + (sum of squares of digits of x) is prime, until the
 *            first x for which f(x) is not prime
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   digit_len, get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

/* Compute the sum of squares of the decimal digits of x. */
static seq_t sumsq_digits(seq_t x)
{
    int dlen = digit_len(x);
    if (dlen <= 0)
        return 0;

    size_t len = (size_t)dlen;
    uint8_t *buf = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (!buf)
        return 0;               /* unlikely to happen in tests */

    size_t used;
    get_digits(x, buf, &used);

    seq_t sum = 0;
    for (size_t i = 0; i < used; ++i)
    {
        seq_t d = (seq_t)buf[i];
        sum += d * d;
    }

    free(buf);
    return sum;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t x = (seq_t)n;

    while (true)
    {
        seq_t sum = sumsq_digits(x);
        seq_t f = x + sum;

        if (!is_prime_big(f))
            break;

        count++;
        x++;                      /* move to the next integer */
    }

    return count;
}
