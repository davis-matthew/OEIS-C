/*
 * A297778  Number of distinct runs in base-10 digits of n.
 * https://oeis.org/A297778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct runs in the decimal representation of
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Runs are maximal consecutive blocks of the same digit; distinct means a pair
   (digit, length) is counted only once. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* not defined for 0; return 0 for safety */
        return (seq_t)0;

    /* Extract decimal digits of n; at most 20 for uint64_t */
    unsigned char digits[21];  /* 0‑20 digits, index 0 unused for convenience */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[++len] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    /* Now digits[1..len] holds digits in reverse order (least significant first) */

    /* Track which (digit, length) pairs have been seen. */
    unsigned char seen[10][21] = {0};
    size_t distinct = 0;

    int run_d = -1;   /* current digit */
    int run_l = 0;    /* current run length */

    /* Process digits from most significant to least. */
    for (size_t i = 0; i < len; ++i) {
        int d = digits[len - i];   /* most significant first */
        if (i == 0) {
            run_d = d;
            run_l = 1;
        } else if (d == run_d) {
            ++run_l;               /* extend current run */
        } else {
            /* End of the current run; record it if new. */
            if (!seen[run_d][run_l]) {
                seen[run_d][run_l] = 1;
                ++distinct;
            }
            run_d = d;
            run_l = 1;
        }
    }

    /* Record the final run. */
    if (!seen[run_d][run_l]) {
        seen[run_d][run_l] = 1;
        ++distinct;
    }

    return (seq_t)distinct;
}
