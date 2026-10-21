/*
 * A297775  Number of distinct runs in base-7 digits of n.
 * https://oeis.org/A297775
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

/* Computation: a(n) = number of distinct runs in the base‑7 representation of n. */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* For n == 0 the base‑7 representation is the single digit 0. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Maximum run length for a 64‑bit integer in base 7 is 64. */
    enum { MAX_RUNLEN = 64 };
    static bool seen[7][MAX_RUNLEN];
    memset(seen, 0, sizeof(seen));

    size_t distinct = 0;
    idx_t m = n;

    unsigned int cur_digit = (unsigned int)(m % 7);
    unsigned int run_len   = 1;
    m /= 7;

    while (m > 0) {
        unsigned int next_digit = (unsigned int)(m % 7);
        if (next_digit == cur_digit) {
            ++run_len;
        } else {
            /* Record the run that just ended. */
            if (!seen[cur_digit][run_len]) {
                seen[cur_digit][run_len] = true;
                ++distinct;
            }
            cur_digit = next_digit;
            run_len   = 1;
        }
        m /= 7;
    }

    /* Record the final run. */
    if (!seen[cur_digit][run_len]) {
        seen[cur_digit][run_len] = true;
        ++distinct;
    }

    return (seq_t)distinct;
}
