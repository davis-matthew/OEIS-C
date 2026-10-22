/*
 * A353145  Decimal repunits written in base 6.
 * https://oeis.org/A353145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007092(A002275(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the decimal integer obtained by writing the n‑digit decimal repunit’s base‑6
 * representation as a decimal number.  For n = 0 the value is 0. */
#include "../utils/seq_pow.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* 10^n as a seq_t, then (10^n – 1)/9 gives the decimal repunit with n ones */
    seq_t pow10 = seq_pow((seq_t)10, n);
    seq_t r = (pow10 - 1) / 9;

    /* The number of base‑6 digits of the repunit is ≈ 1.285⋅n, so 20000 suffices
     * for n ≤ 10000.  The buffer is static to keep the interface purely C. */
    unsigned char digits[20000];
    size_t len = 0;

    while (r > 0)
    {
        digits[len++] = (unsigned char)(r % (seq_t)6);
        r = r / (seq_t)6;
    }

    /* Build the result by reading the digits from most significant to least
     * significant, each time multiplying by 10 and adding the next digit. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i)
        result = result * (seq_t)10 + digits[len - 1 - i];

    return result;
}
