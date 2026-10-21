/*
 * A297773  Number of distinct runs in base-5 digits of n.
 * https://oeis.org/A297773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct runs of equal digits in the base‑5
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case for n == 0: representation is "0" → length 1 run of digit 0 */
    if (n == 0)
        return (seq_t)1;

    /* Convert n to base‑5, least‑significant digit first */
    idx_t tmp = n;
    uint8_t digits[32];          /* 2^64 < 5^28, so 32 slots are more than enough */
    size_t len = 0;
    while (tmp != 0) {
        digits[len++] = tmp % 5;
        tmp /= 5;
    }

    /* Bitmask of run lengths seen for each digit (1 ↔ bit 0, 2 ↔ bit 1, …) */
    unsigned int seen[5] = {0};
    int      cur_digit = -1;
    unsigned int cur_len = 0;

    /* Scan digits from most significant to least significant */
    for (size_t i = 0; i < len; ++i) {
        int d = digits[len - 1 - i];
        if (cur_digit == d) {
            ++cur_len;
        } else {
            if (cur_digit != -1) {
                seen[cur_digit] |= (1U << (cur_len - 1));
            }
            cur_digit = d;
            cur_len   = 1;
        }
    }
    /* Record the final run */
    if (cur_digit != -1) {
        seen[cur_digit] |= (1U << (cur_len - 1));
    }

    /* Count distinct (digit, length) pairs */
    seq_t count = 0;
    for (int d = 0; d < 5; ++d) {
        unsigned int mask = seen[d];
        while (mask) {
            mask &= mask - 1;  /* clear lowest set bit */
            ++count;
        }
    }
    return count;
}
