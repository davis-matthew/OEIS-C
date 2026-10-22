/*
 * A342845  Number of distinct even numbers visible as proper substrings of n.
 * https://oeis.org/A342845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct even integers that appear as proper
 *            substrings of the decimal representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Extract decimal digits of n into an array in most‑significant first order */
    unsigned char digits[20];   /* enough for 64‑bit values */
    size_t len = 0;

    if (n == 0) {            /* special case: single digit "0" */
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to get most significant digit first */
        for (size_t i = 0; i < len / 2; ++i) {
            unsigned char t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* At most len*(len-1)/2 distinct proper substrings; use a small fixed array */
    seq_t seen[200];
    size_t seen_cnt = 0;

    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * (seq_t)10 + (seq_t)digits[j];
            if (i == 0 && j == len - 1) {
                /* skip the whole string – proper substring only */
                continue;
            }
            if ((val & (seq_t)1) != 0) {
                /* not even */
                continue;
            }
            /* check if val already recorded */
            int exists = 0;
            for (size_t k = 0; k < seen_cnt; ++k) {
                if (seen[k] == val) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                seen[seen_cnt++] = val;
            }
        }
    }
    return (seq_t)seen_cnt;
}
