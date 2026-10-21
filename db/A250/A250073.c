/*
 * A250073  Powers of 2 written in base 60, concatenating the decimal values
 *          of the sexagesimal digits.
 * https://oeis.org/A250073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal concatenation of 2^n expressed in base‑60. Each
 *            base‑60 digit is written as two decimal digits (as on a digital
 *            clock). Leading zeros are removed for the final decimal integer
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n in the same integer type. */
    seq_t val = pow2(n);

    /* Convert to base‑60 digits (least significant first). */
    enum { MAX_DIGITS = 2048 };          /* +1 safety for n up to ~10000 */
    uint32_t stack[MAX_DIGITS];
    size_t pos = 0;
    const seq_t base = 60;

    while (val > 0) {
        seq_t q = val / base;
        seq_t r = val % base;
        stack[pos++] = (uint32_t)r;
        val = q;
    }

    /* Assemble decimal concatenation, trimming leading zeros. */
    seq_t result;
    seq_t msd = (seq_t)stack[pos - 1];   /* most significant digit */
    result = msd;                       /* a digit <10 already has no leading 0 */

    /* Append remaining digits, each contributing two decimal places. */
    if (pos > 1) {
        for (size_t i = pos - 2; ; --i) {
            result = result * 100 + (seq_t)stack[i];
            if (i == 0)
                break;
        }
    }

    return result;
}
