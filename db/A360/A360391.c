/*
 * A360391  a(n) is the number of distinct sums of nonempty subsets of the
 *          digits of n.
 * https://oeis.org/A360391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct sums of non‑empty subsets of the
 *            decimal digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* The maximum possible sum of decimal digits for a 64‑bit integer is
   9 * 20 = 180. */
#define MAX_SUM 180

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into a temporary array. */
    uint8_t digits[20];
    size_t len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* DP array: poss[s] is true if sum s is attainable. */
    bool poss[MAX_SUM + 1] = {false};
    size_t max_sum = 0;                    /* current maximum achievable sum */

    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digits[i];

        /* Update sums in decreasing order to avoid re‑using the same digit. */
        for (size_t s = max_sum; s > 0; --s) {
            if (poss[s]) {
                poss[s + d] = true;
            }
        }
        /* The subset containing only this digit. */
        poss[d] = true;

        max_sum += d;
    }

    /* Count distinct sums (non‑empty subsets always have at least one digit,
       so sum 0 can only arise from a digit 0). */
    seq_t count = 0;
    for (size_t s = 0; s <= max_sum; ++s) {
        if (poss[s]) {
            ++count;
        }
    }
    return count;
}
