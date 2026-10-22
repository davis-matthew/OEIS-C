/*
 * A329424  Starting from n: as long as the decimal representation starts with
 *          a positive multiple of 3, divide the largest such prefix by 3;
 *          a(n) corresponds to the final value.
 * https://oeis.org/A329424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   defined by iteratively replacing the largest left‑hand decimal
 *            prefix that is a positive multiple of 3 by that prefix divided
 *            by 3. The result is the final value with no such prefix
 *            remaining
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t current = (seq_t)n;           /* start with the index as a number */
    /* buffer for decimal digits, least significant first;
       64 digits can hold any 64‑bit number, and is more than enough
       for the implicit small‑integer limits of the seq_t type.   */
    static uint8_t digits[64];

    for (;;) {
        /* convert current to decimal digits */
        seq_t t = current;
        size_t len = 0;
        if (t == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            while (t != 0) {
                digits[len++] = (uint8_t)(t % 10);
                t /= 10;
            }
        }

        /* find the longest prefix (from most significant side)
           that is a positive multiple of 3 */
        seq_t best_prefix = 0;
        size_t best_idx = len;          /* sentinel: none found yet */

        seq_t pref = 0;
        for (size_t i = len; i-- > 0; ) {   /* i: index of most significant digit */
            pref = pref * 10 + digits[i];
            if (pref % 3 == 0 && pref != 0) {
                best_idx = i;
                best_prefix = pref;
            }
        }

        /* if no prefix found, we are finished */
        if (best_idx == len)
            break;

        /* compute the suffix value (digits right of the chosen prefix) */
        seq_t suffix = 0;
        for (size_t j = best_idx; j-- > 0; ) {   /* from the most significant digit
                                                    of the suffix down to least   */
            suffix = suffix * 10 + digits[j];
        }

        /* compute 10^suffix_len */
        size_t suffix_len = best_idx;
        seq_t pow10 = 1;
        for (size_t i = 0; i < suffix_len; ++i)
            pow10 *= 10;

        /* form new value: (prefix/3) concatenated with suffix */
        seq_t new_value = (best_prefix / 3) * pow10 + suffix;
        current = new_value;
    }

    return current;
}
