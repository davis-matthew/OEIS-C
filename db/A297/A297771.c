/*
 * A297771  Number of distinct runs in base-3 digits of n.
 * https://oeis.org/A297771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: number of distinct runs in base‑3 digits of n */
#include <stdint.h>
#include <stdbool.h>

static const int MAX_DIGITS = 64;          /* enough for 3^64 > 2^64 */

seq_t a(idx_t n)
{
    /* Convert n to base‑3 digits in MSB → LSB order */
    int digits[MAX_DIGITS];
    int len = 0;
    idx_t t = n;
    do {
        digits[len++] = (int)(t % 3);
        t /= 3;
    } while (t > 0);

    /* Reverse to MSB first */
    for (int i = 0; i < len / 2; ++i) {
        int tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }

    /* Count distinct runs (digit, run length) */
    struct Run { int digit; int len; };
    struct Run runs[MAX_DIGITS];
    int distinct = 0;

    for (int i = 0; i < len; ) {
        int d = digits[i];
        int j = i + 1;
        while (j < len && digits[j] == d) j++;

        /* Check if this (digit, length) pair already seen */
        bool seen = false;
        for (int k = 0; k < distinct; ++k) {
            if (runs[k].digit == d && runs[k].len == (j - i)) {
                seen = true;
                break;
            }
        }
        if (!seen) {
            runs[distinct].digit = d;
            runs[distinct].len = j - i;
            ++distinct;
        }
        i = j;
    }

    return (seq_t)distinct;
}
