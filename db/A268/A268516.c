/*
 * A268516  Take alternate digits of 2^n.
 * https://oeis.org/A268516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = take the alternate (odd‑positioned) decimal digits of
 *            2^n, starting from the most significant digit
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 62, u64 n <= 127,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Rough estimate of decimal digits of 2^n:
       floor(n * log10(2)) + 1,  with log10(2) ≈ 30103/100000 ≈ 0.30103. */
    uint64_t temp = (uint64_t)n * 30103ull;  /* 30103 ≈ log10(2)*100000 */
    size_t capacity = (size_t)(temp / 100000ull + 10); /* safety margin */

    /* Allocate storage for decimal digits, least‑significant first. */
    uint8_t *d = (uint8_t *)malloc(capacity);
    if (!d)
        return 0;   /* allocation failure → zero */

    size_t len = 1;
    d[0] = 1;           /* 2^0 = 1 */

    /* Multiply by 2, n times. */
    for (idx_t k = 0; k < n; ++k) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint16_t tmp = (uint16_t)d[j] * 2 + carry;   /* max 9*2+1 = 19 */
            d[j] = (uint8_t)(tmp % 10);
            carry = (uint8_t)(tmp / 10);
        }
        if (carry) {
            if (len == capacity) {
                capacity = capacity * 2;
                uint8_t *nd = (uint8_t *)realloc(d, capacity);
                if (!nd) { free(d); return 0; }
                d = nd;
            }
            d[len++] = carry;   /* carry is always 0 or 1 */
        }
    }

    /* Build the result from the odd‑positioned digits (MSD position = 1). */
    seq_t result = 0;
    for (int64_t i = (int64_t)len - 1; i >= 0; --i) {
        /* position from MSB: pos = len - i */
        if (((len - i) & 1) == 1) {
            result = result * 10 + d[i];
        }
    }

    free(d);
    return result;
}
