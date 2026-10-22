/*
 * A382465  Positive integers such that every even digit except the first is
 *          immediately preceded by a smaller digit.
 * https://oeis.org/A382465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th positive integer whose decimal digits satisfy *
 *            that every even digit except the first is immediately preceded
 *            by a * smaller digit
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_dp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_dp.h"

#define MAX_LEN 32                       /* sufficient for n up to at least 1e4 */

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* the offset is 1‑based */

    idx_t cum = 0;                      /* terms seen so far */
    idx_t len = 1;                      /* current candidate length */

    static idx_t f[MAX_LEN][10];       /* DP table reused in each loop */

    /* Find the length L that contains the n-th term */
    while (1) {
        idx_t rem = len - 1;             /* remaining digits after the first */

        build_dp(rem, f);

        /* Count numbers of exact length 'len' */
        idx_t count_len = 0;
        for (int first = 1; first <= 9; ++first) count_len += f[rem][first];

        if (cum + count_len >= n) break;   /* n lies within this block */
        cum += count_len;
        ++len;
    }

    /* Offset within the set of numbers of this length (0‑based) */
    idx_t offset = n - cum - 1;
    idx_t rem = len - 1;                  /* digits left after the first */

    /* Pick the first digit */
    int first_digit = 0;
    for (int d = 1; d <= 9; ++d) {
        idx_t cnt = f[rem][d];
        if (offset >= cnt) {
            offset -= cnt;                  /* skip this block */
        } else {
            first_digit = d;
            break;
        }
    }

    seq_t val = (seq_t)first_digit;
    int prev = first_digit;                /* previous digit for the next loop */

    /* Build the remaining digits */
    while (rem > 0) {
        --rem;                               /* one digit less to place */
        for (int d = 0; d <= 9; ++d) {
            if ((d & 1) == 0 && d <= prev) continue;   /* even digit restriction */
            idx_t cnt = f[rem][d];
            if (offset >= cnt) {
                offset -= cnt;
            } else {
                val = val * 10 + d;
                prev = d;
                break;
            }
        }
    }

    return val;
}
