/*
 * A297774  Number of distinct runs in base-6 digits of n.
 * https://oeis.org/A297774
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

/* Computation: a(n) = the number of distinct runs in the base‑6 representation of n.
 * A run is a maximal consecutive block of equal digits; two runs are considered
 * distinct if they differ in digit or length. This routine scans the base‑6
 * digits of n, records each run, and counts how many unique (digit, length)
 * pairs occur. */
#include <stddef.h>   /* for size_t */

struct run {
    int dig;
    int len;
};

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0; return 0 for safety */
        return 0;

    /* Convert n to base‑6 digits (least‑significant first). */
    int digits[70];                      /* 70 > max digits for 64‑bit n */
    idx_t tmp = n;
    size_t dlen = 0;
    do {
        digits[dlen++] = (int)(tmp % 6);
        tmp /= 6;
    } while (tmp > 0);

    /* Array of previously seen runs (max dlen runs). */
    struct run seen[70];
    size_t seen_cnt = 0;

    /* Variables to keep track of the current run. */
    int prev_digit = -1;    /* sentinel for “no previous digit” */
    int run_len = 0;

    /* Scan digits from least‑significant to most‑significant. */
    for (size_t i = 0; i < dlen; ++i) {
        int d = digits[i];
        if (i == 0 || d != prev_digit) {                /* new run starts */
            if (i > 0) {                                /* finish previous run */
                int found = 0;
                for (size_t j = 0; j < seen_cnt; ++j) {
                    if (seen[j].dig == prev_digit &&
                        seen[j].len == run_len) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    seen[seen_cnt].dig = prev_digit;
                    seen[seen_cnt].len = run_len;
                    ++seen_cnt;
                }
            }
            prev_digit = d;
            run_len = 1;
        } else {                                        /* same digit continues */
            ++run_len;
        }
    }

    /* Store the last run. */
    int found = 0;
    for (size_t j = 0; j < seen_cnt; ++j) {
        if (seen[j].dig == prev_digit && seen[j].len == run_len) {
            found = 1;
            break;
        }
    }
    if (!found) {
        seen[seen_cnt].dig = prev_digit;
        seen[seen_cnt].len = run_len;
        ++seen_cnt;
    }

    return (seq_t)seen_cnt;
}
