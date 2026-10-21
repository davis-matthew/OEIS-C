/*
 * A275982  Decimal expansion of 2^21701 - 1, the 25th Mersenne prime
 *          A000668(25).
 * https://oeis.org/A275982
 *
 * Offset:    6533    (a(6533) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = decimal digit (n - 6533 + 1) of 2^21701 - 1
 * Range:     u8 n <= 9788, u16 n <= 9788, u32 n <= 9788, u64 n <= 9788,
 *            u128 n <= 9788, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function precomputes the decimal expansion of 2^21701 - 1
   the first time it is called, then returns the requested digit. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint8_t _mersenne_digits[6540];
static size_t   _mersenne_len   = 0;
static bool     _mersenne_init  = false;

/* Compute 2^21701 - 1 in decimal, least‑significant digit first. */
static void init_mersenne_digits(void)
{
    uint8_t *d = _mersenne_digits;
    size_t len = 1;
    d[0] = 1;                               /* start with 1 */

    /* Multiply by 2 one hundred twenty‑seven thousand one times. */
    for (idx_t k = 0; k < 21701; ++k)
    {
        uint16_t carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            uint16_t tmp = d[i] * 2u + carry;
            d[i]        = (uint8_t)(tmp % 10u);
            carry       = tmp / 10u;
        }
        if (carry)                         /* append possible carry digit */
            d[len++] = (uint8_t)carry;
    }

    /* Subtract 1 from the least‑significant digit. */
    size_t i = 0;
    while (i < len)
    {
        if (d[i] > 0)
        {
            d[i] -= 1;
            break;
        }
        d[i] = 9;  /* borrow */
        ++i;
    }

    _mersenne_len  = len;
    _mersenne_init = true;
}

seq_t a(idx_t n)
{
    /* Sequence offset is 6533. For indices below the offset the value
       is unspecified; we return 0. */
    if (n < 6533)
        return (seq_t)0;

    if (!_mersenne_init)
        init_mersenne_digits();

    /* Position in the digit array, 0‑based from the leftmost digit. */
    idx_t pos = n - 6533;                     /* 0 for first digit */
    if ((size_t)pos >= _mersenne_len)
        return (seq_t)0;                      /* out of bounds */

    /* Convert from leftmost to least‑significant index. */
    size_t idx = _mersenne_len - 1 - (size_t)pos;
    return (seq_t)_mersenne_digits[idx];
}
