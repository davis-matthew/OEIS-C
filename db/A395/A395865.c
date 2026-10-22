/*
 * A395865  Decimal expansion of 2^1024 - 2^971.
 * https://oeis.org/A395865
 *
 * Offset:    309    (a(309) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   a(n) = decimal digit at position n of 2^1024 - 2^971
 * Range:     u8 n <= 617, u16 n <= 617, u32 n <= 617, u64 n <= 617,
 *            u128 n <= 617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

seq_t a(idx_t n)
{
    /* digits holds the decimal expansion of 2^1024 - 2^971,
       most significant digit first. */
    static char digits[400];
    static size_t dig_len = 0;
    static bool ready = false;

    if (!ready) {
        /* representation of the number in base 1e9, little-endian */
        uint32_t dec[128] = {0};
        size_t dec_len = 1; /* start with value 0 */

        /* process the 53 leading 1‑bits (bits 971 … 1023) */
        for (int i = 0; i < 53; ++i) {
            /* dec = dec * 2 */
            uint64_t carry = 0;
            for (size_t j = 0; j < dec_len; ++j) {
                uint64_t val = ((uint64_t)dec[j] << 1) + carry;
                dec[j] = (uint32_t)(val % 1000000000);
                carry   = val / 1000000000;
            }
            if (carry) dec[dec_len++] = (uint32_t)carry;

            /* dec = dec + 1 */
            uint64_t add = 1;
            size_t j = 0;
            while (add && j < dec_len) {
                uint64_t val = (uint64_t)dec[j] + add;
                dec[j] = (uint32_t)(val % 1000000000);
                add    = val / 1000000000;
                ++j;
            }
            if (add) dec[dec_len++] = (uint32_t)add;
        }

        /* process the remaining 971 zero‑bits (bits 0 … 970) */
        for (int i = 0; i < 971; ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < dec_len; ++j) {
                uint64_t val = ((uint64_t)dec[j] << 1) + carry;
                dec[j] = (uint32_t)(val % 1000000000);
                carry   = val / 1000000000;
            }
            if (carry) dec[dec_len++] = (uint32_t)carry;
            /* no addition of a bit */
        }

        /* convert the base‑1e9 digits to a decimal string */
        char buf[400];
        size_t pos = 0;
        for (int i = (int)dec_len - 1; i >= 0; --i) {
            if (i == (int)dec_len - 1)
                pos += sprintf(buf + pos, "%u", dec[i]);
            else
                pos += sprintf(buf + pos, "%09u", dec[i]);
        }
        for (size_t i = 0; i < pos; ++i) digits[i] = buf[i];
        dig_len = pos;
        ready   = true;
    }

    if (n < (idx_t)309) return (seq_t)0;        /* undefined below the offset */
    size_t idx = (size_t)(n - (idx_t)309);       /* zero‑based index in digits */
    if (idx >= dig_len) return (seq_t)0;        /* beyond the known digits */
    return (seq_t)(digits[idx] - '0');
}
