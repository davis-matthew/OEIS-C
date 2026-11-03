/*
 * A323835  Start with n and repeatedly double it and apply the "delete
 *          multiple digits" map m -> A320485(m); a(n) is the number of steps
 *          needed to reach either 0 or 1, or -1 if neither 0 nor 1 is ever
 *          reached.
 * https://oeis.org/A323835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 999, u64 n <= 999, u128 n <= 999,
 *            big any n
 * Validated: standard, pass, 86 terms through n = 982 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A323835 – Compute a(n) by iterating: start at n, repeatedly double
   the value and delete all digits that occur more than once in its decimal
   representation (keeping only digits appearing exactly once).  a(n) is the
   number of such iterations required to reach 0 or 1, or (unsigned)-1 if
   neither 0 nor 1 is ever reached.  The function assumes that the
   *delete_multiple_digits* mapping (A320485) is defined below. */
#include <stddef.h>
#include <stdbool.h>

/* Delete digits that appear more than once in a number.
   Digits that appear exactly once are kept in their original order.
   If no digits are kept the result is 0. */
static seq_t delete_multiple_digits(seq_t m)
{
    unsigned int counts[10] = {0};
    if (m == 0) {
        counts[0] = 1;
    } else {
        seq_t temp = m;
        while (temp > 0) {
            unsigned int d = (unsigned int)(temp % 10);
            counts[d]++;
            temp /= 10;
        }
    }

    /* Store digits in reverse order to reconstruct in original order later. */
    unsigned char digits[128];
    size_t nd = 0;
    if (m == 0) {
        digits[nd++] = 0;
    } else {
        seq_t temp = m;
        while (temp > 0) {
            digits[nd++] = (unsigned char)(temp % 10);
            temp /= 10;
        }
    }

    /* Build the result keeping only digits that appear once. */
    seq_t result = 0;
    for (size_t i = nd; i-- > 0; ) {
        unsigned int d = digits[i];
        if (counts[d] == 1) {
            result = result * 10 + d;
        }
    }
    return result;
}

/* a(n) – number of steps to reach 0 or 1 by the map described above,
   or -1 if a cycle occurs that never contains 0 or 1. */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    /* A small buffer is sufficient because the sequence is known to take
       at most 64 steps for any n.  The buffer keeps already seen values to
       detect cycles. */
    const size_t VIS_MAX = 200;
    seq_t visited[VIS_MAX];
    size_t vis_cnt = 0;
    seq_t cur = (seq_t)n;
    visited[vis_cnt++] = cur;
    unsigned int steps = 0;

    while (true) {
        /* Apply the map: double and then delete repeated digits. */
        cur = cur * 2;
        cur = delete_multiple_digits(cur);
        steps++;

        /* Check for terminating conditions. */
        if (cur == 0 || cur == 1) {
            return (seq_t)steps;
        }

        /* Detect a repeat – cycle found. */
        for (size_t i = 0; i < vis_cnt; ++i) {
            if (visited[i] == cur) {
                return (seq_t)-1;  /* never reaches 0 or 1 */
            }
        }

        if (vis_cnt >= VIS_MAX) {
            /* Should not happen because steps <= 64, but guard against overflow. */
            return (seq_t)-1;
        }

        visited[vis_cnt++] = cur;
    }
}
