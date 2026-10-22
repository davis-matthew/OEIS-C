/*
 * A319724  Write n in 7-ary, sort digits into decreasing order.
 * https://oeis.org/A319724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 2400, u32 n <= 2400, u64 n <= 2400,
 *            u128 n <= 2400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A319724: Write n in 7‑ary, sort digits into decreasing order
   and interpret the result as a base‑7 number.  The algorithm sorts the
   base‑7 digits using a small count array (since digits are 0‑6) and
   recombines them into the final integer.  This works for any n >= 0
   and requires only O(log_7 n) time and O(1) extra space. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Collect base‑7 digits of n */
    uint8_t digits[64];   /* log_7(2^64) < 20, buffer sufficient */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 7);
        tmp /= 7;
    }

    /* Count occurrences of each digit (0‑6) */
    uint8_t cnt[7] = {0};
    for (size_t i = 0; i < len; ++i) {
        cnt[digits[i]]++;
    }

    /* Reconstruct number from highest digit to lowest */
    seq_t result = 0;
    for (int d = 6; d >= 0; --d) {
        for (uint8_t c = cnt[d]; c > 0; --c) {
            result = result * (seq_t)7 + (seq_t)d;
        }
    }

    return result;
}
