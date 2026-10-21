/*
 * A242367  Numbers whose parity matches only their least significant decimal
 *          digit.
 * https://oeis.org/A242367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 4904, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: Numbers whose parity matches only their least significant decimal digit. */

#include <stdbool.h>

static bool valid(idx_t num)
{
    idx_t last_digit = num % 10;
    bool parity = (bool)(last_digit & 1u);
    num /= 10;
    while (num > 0) {
        unsigned digit = (unsigned)(num % 10);
        if ((digit & 1u) == parity) return false;
        num /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Determine the length L of the n‑th term. */
    idx_t len = 1;
    idx_t cumulative = 9;               /* total up to length 1 */
    if (n <= cumulative) return (seq_t)n;   /* first nine terms are 1..9 */

    idx_t count = cumulative;           /* 9*5^(len-1) for current len */

    while (true) {
        len++;                           /* increase length */
        count *= 5;                      /* count for new length */
        if (cumulative + count >= n) break;
        cumulative += count;
    }

    /* Offset within the numbers of length len (0‑based). */
    idx_t offset = n - cumulative - 1;

    /* Starting value of len‑digit numbers. */
    idx_t start = 1;
    for (idx_t i = 1; i < len; ++i) start *= 10;

    /* Find the offset‑th valid number in numeric order. */
    for (idx_t num = start; ; ++num) {
        if (valid(num)) {
            if (offset == 0) return (seq_t)num;
            --offset;
        }
    }
}
