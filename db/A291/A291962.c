/*
 * A291962  Decimal repunits written in base 2.
 * https://oeis.org/A291962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007088(A002275(n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS A291962: decimal repunits written in base 2. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Step 1: build the decimal repunit r = (10^n - 1)/9. */
    seq_t repunit = 0;
    for (idx_t i = 0; i < n; ++i)
        repunit = repunit * 10 + 1;

    /* Step 2: extract bits of repunit (least‑significant first). */
    size_t capacity = 64;
    unsigned char *bits = (unsigned char *)malloc(capacity);
    size_t count = 0;
    seq_t temp = repunit;
    while (temp > 0)
    {
        unsigned char bit = (unsigned char)(temp % 2);
        if (count == capacity)
        {
            capacity <<= 1;
            unsigned char *newp = (unsigned char *)realloc(bits, capacity);
            bits = newp;
        }
        bits[count++] = bit;
        temp = temp / 2;
    }

    /* Step 3: interpret the bits as a decimal number. */
    seq_t result = 0;
    for (size_t i = count; i > 0; --i)
        result = result * 10 + bits[i - 1];

    free(bits);
    return result;
}
