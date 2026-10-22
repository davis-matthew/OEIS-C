/*
 * A352416  A permutation related to minimal Pell representations: append a 0
 *          after each 2 in the ternary expansion of n, and then replace each
 *          place value, say 3^k with k >= 0, by A000129(k+1).
 * https://oeis.org/A352416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ d'_k * Pell(k+1), where d'_k are the digits of the
 *            ternary expansion of n with every digit '2' followed by an
 *            inserted '0'. Pell(m) is the m‑th Pell number (A000129)
 * Range:     u8 n <= 52, u16 n <= 1456, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1. Convert n to base‑3 digits (least significant first). */
    uint8_t digits[128];
    size_t dlen = 0;
    if (n == 0) {
        digits[dlen++] = 0;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[dlen++] = tmp % 3;
            tmp /= 3;
        }
    }

    /* 2. Build the modified digit sequence:
          from most significant to least, insert a 0 after every 2. */
    uint8_t modDigits[128];
    size_t mlen = 0;
    for (size_t i = dlen; i-- > 0;) {          /* iterate msb → lsb */
        modDigits[mlen++] = digits[i];
        if (digits[i] == 2) modDigits[mlen++] = 0;
    }

    /* 3. Precompute Pell numbers up to mlen for the weight coefficients. */
    seq_t pell[129];
    pell[0] = 0;          /* unused */
    pell[1] = 1;
    if (mlen >= 2) pell[2] = 2;
    for (size_t k = 3; k <= mlen; ++k) {
        pell[k] = pell[k-1] + pell[k-1] + pell[k-2];   /* Pell(k) = 2*Pell(k-1)+Pell(k-2) */
    }

    /* 4. Compute the weighted sum. */
    seq_t result = 0;
    for (size_t j = 0; j < mlen; ++j) {
        size_t pos = mlen - 1 - j;  /* position from least significant digit */
        seq_t d = (seq_t)modDigits[j];
        if (d == 0) continue;
        result += d * pell[pos + 1];
    }
    return result;
}
