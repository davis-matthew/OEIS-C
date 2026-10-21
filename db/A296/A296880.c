/*
 * A296880  Numbers whose base-9 digits d(m), d(m-1), ..., d(0) have #(pits) >
 *          #(peaks); see Comments.
 * https://oeis.org/A296880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296880: numbers whose base‑9 digits d(m)…d(0) satisfy
   #(pits) > #(peaks).  The function below enumerates the sequence
   by testing consecutive natural numbers.  It works with any
   integer‐like seq_t, including arbitrary‑precision types; in C++17
   the same source compiles.  The algorithm runs in O(n·log₉n) time
   and uses only stack storage. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* We enumerate natural numbers from 1 until we have found the
       n‑th term satisfying #(pits) > #(peaks).  No caching is
       required; each call recomputes from the start.  This simple
       approach satisfies the time limit for n ≤ 10 000.  */

    /* A buffer big enough for the base‑9 representation of any
       seq_t that fits in 128 bits (and far more for arbitrary‑precision
       types used in the C++ build).  200 digits more than enough. */
    uint8_t digits[200];
    size_t len;
    idx_t found = 0;
    seq_t candidate = 1;   /* start from 1 (a(1) is the first term) */

    while (true) {
        /* Convert candidate to base‑9 digits in reverse order */
        seq_t temp = candidate;
        len = 0;
        do {
            /* temp % 9 and temp / 9 work for any seq_t that overloads
               these operators (integral types or big‑int classes).  */
            digits[len++] = (uint8_t)(temp % (seq_t)9);
            temp /= (seq_t)9;
        } while (temp != (seq_t)0);

        /* Count pits and peaks */
        size_t pits = 0, peaks = 0;
        for (size_t i = 1; i + 1 < len; ++i) {
            if (digits[i-1] > digits[i] && digits[i] < digits[i+1]) {
                ++pits;
            } else if (digits[i-1] < digits[i] && digits[i] > digits[i+1]) {
                ++peaks;
            }
        }

        if (pits > peaks) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }

        ++candidate;
    }
}
