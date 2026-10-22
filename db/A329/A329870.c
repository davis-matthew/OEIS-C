/*
 * A329870  Runs-resistance of the binary expansion of n without the first
 *          digit.
 * https://oeis.org/A329870
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of applications of the run-length
 * reduction on the binary expansion of n without its first digit. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the offset says the result is irrelevant; return 0. */
    if (n < 2ULL)
        return (seq_t)0;

    /* Determine the number of bits after the leading 1. */
    idx_t tmp = n;
    int len = 0;           /* length of the reduced binary sequence */
    while (tmp >>= 1)      /* shift until only the leading bit remains */
        ++len;

    /* Extract the bits following the most significant 1. */
    int digits[64];
    for (int i = 0; i < len; ++i) {
        int shift = len - i - 1;
        digits[i] = (int)((n >> (uint64_t)shift) & 1ULL);
    }

    int count = 0;
    /* Iterate until the sequence becomes a singleton. */
    while (len > 1) {
        int cur[64];
        int curlen = 0;
        int run = 1;

        for (int j = 1; j < len; ++j) {
            if (digits[j] == digits[j - 1]) {
                ++run;
            } else {
                cur[curlen++] = run;
                run = 1;
            }
        }
        cur[curlen++] = run;               /* last run */

        /* Prepare for the next iteration. */
        for (int k = 0; k < curlen; ++k)
            digits[k] = cur[k];
        len = curlen;
        ++count;
    }

    return (seq_t)count;
}
