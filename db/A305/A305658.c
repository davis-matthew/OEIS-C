/*
 * A305658  Powers of 3 in base 3/2.
 * https://oeis.org/A305658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A024629(A000244(n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   pow10, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"   /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/pow10.h"    /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    /* 1. Compute 2^n as a seq_t value. */
    seq_t val = seq_pow((seq_t)2, n);

    /* 2. Extract base‑3/2 digits (least significant first). */
    size_t cap = (size_t)n * 2 + 1;          /* enough space for digits */
    uint8_t *dig = (uint8_t *)malloc(cap);
    size_t len = 0;

    while (val != (seq_t)0) {
        seq_t rem = val % (seq_t)3;           /* digit = val mod 3   */
        dig[len++] = (uint8_t)rem;
        val = ((val - rem) * (seq_t)2) / (seq_t)3;
    }

    /* 3. Build the decimal number from most significant digit. */
    seq_t res = 0;
    for (size_t i = len; i-- > 0; ) {
        res = res * (seq_t)10 + (seq_t)dig[i];
    }

    /* 4. Append n trailing zeros by multiplying by 10^n. */
    if (n > 0) {
        seq_t pow10n = pow10(n);
        res = res * pow10n;
    }

    free(dig);
    return res;
}
