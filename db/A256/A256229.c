/*
 * A256229  Powering the decimal digits of n (right-associative) with 0^0 = 1
 *          by convention.
 * https://oeis.org/A256229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hear, look
 * Formula:   a(n) = right‑associative powering of the decimal digits of n,
 *            with 0^0 defined as 1 by convention
 * Range:     u8 n <= 27, u16 n <= 47, u32 n <= 224, u64 n <= 225,
 *            u128 n <= 226, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n = 0 is outside the defined range, but return 0 safely. */
    if (n == 0)
        return (seq_t)0;

    /* Collect digits of n (least significant first).  uint64_t fits at
       most 20 decimal digits. */
    idx_t x = n;
    uint8_t digs[20];
    size_t len = 0;
    do {
        digs[len++] = (uint8_t)(x % 10);
        x /= 10;
    } while (x);

    /* Start the exponent tower with the rightmost digit. */
    seq_t result = (seq_t)digs[0];

    /* Process remaining digits from right to left (i.e., from least to most
       significant).  Each step replaces result with base ^ result. */
    for (size_t i = 1; i < len; ++i) {
        seq_t base = (seq_t)digs[i];
        seq_t exp  = result;

        if (base == (seq_t)0) {
            if (exp == (seq_t)0)
                result = (seq_t)1;   /* 0^0 = 1 by convention */
            else
                result = (seq_t)0;   /* 0^k = 0 for k > 0 */
        } else if (exp == (seq_t)0) {
            result = (seq_t)1;       /* k^0 = 1 for k > 0 */
        } else {
            result = big_pow(base, exp);
        }
    }

    return result;
}
