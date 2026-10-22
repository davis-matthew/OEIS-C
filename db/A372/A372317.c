/*
 * A372317  For any n > 0 with leading ternary digit h = A122586(n), reverse
 *          digits in blocks in ternary expansion of n where blocks are
 *          separated by h's; a(0) = 0.
 * https://oeis.org/A372317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by taking the base‑3 representation of n, with
 *            leading digit h, reversing each block between consecutive h's,
 *            and keeping the h's in their positions; a(0)=0
 * Range:     u8 n <= 244, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Maximum number of base‑3 digits for a 64‑bit integer is 40; use a
       small fixed buffer to avoid dynamic allocation. */
    const size_t MAX_DIGITS = 65;
    unsigned digits[MAX_DIGITS];
    unsigned res_digits[MAX_DIGITS];

    /* Convert n to base‑3 digits (least‑significant first) */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        digits[len++] = tmp % 3;
        tmp /= 3;
    }

    /* Reverse to most‑significant first */
    for (size_t i = 0; i < len / 2; ++i) {
        unsigned t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    unsigned h = digits[0];          /* leading digit */
    res_digits[0] = h;               /* leading digit stays unchanged */

    size_t pos_res = 1;              /* next position to write in res_digits */
    size_t block_start = 1;          /* start of current block in digits[] */

    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == h) {
            /* Reverse block digits[block_start .. i-1] */
            for (size_t j = i; j > block_start; --j) {
                res_digits[pos_res++] = digits[j - 1];
            }
            /* Write the separator digit h */
            res_digits[pos_res++] = h;
            block_start = i + 1;
        }
    }
    /* Final block */
    for (size_t j = len; j > block_start; --j) {
        res_digits[pos_res++] = digits[j - 1];
    }

    /* Convert result digits back to integer */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 3 + (seq_t)res_digits[i];
    }

    return result;
}
