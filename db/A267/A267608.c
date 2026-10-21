/*
 * A267608  Binary representation of the n-th iteration of the "Rule 182"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   rule182
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(n) is the decimal integer whose digits are the binary pattern
   obtained after n iterations of elementary cellular automaton Rule 182,
   starting from a single ON cell.  The bits are written left‑to‑right, the
   most significant bit being the leftmost cell.  The integer is NOT the
   value of that binary string – it is the number formed by those bits as
   decimal digits (e.g. bits 1 0 1 0 1 give 10101).  Example: a(0)=1,
   a(1)=111, a(2)=10101. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/rule182.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Maximum number of cells after n steps is 2*n + 1 */
    size_t max_len = 2ull * (size_t)n + 3;  /* safety margin */

    uint8_t *cur  = (uint8_t *)calloc(max_len, 1);
    uint8_t *next = (uint8_t *)calloc(max_len, 1);
    if (!cur || !next) { /* OOM protection */
        free(cur);
        free(next);
        return (seq_t)0;
    }

    cur[0] = 1;
    size_t len = 1;        /* current number of cells */

    for (idx_t step = 1; step <= n; ++step) {
        size_t new_len = len + 2;   /* two new cells each step */

        for (size_t j = 0; j < new_len; ++j) {
            uint8_t left   = 0;
            uint8_t center = 0;
            uint8_t right  = 0;

            if (j >= 2 && (j - 2) < len)  left   = cur[j - 2];
            if (j >= 1 && (j - 1) < len)  center = cur[j - 1];
            if (j < len)                 right  = cur[j];

            next[j] = rule182(left, center, right) ? 1 : 0;
        }

        /* swap buffers and update length */
        size_t tmp   = len;
        len          = new_len;
        uint8_t *tmp_b = cur;
        cur           = next;
        next          = tmp_b;
    }

    /* Convert the binary pattern into a decimal integer formed by its bits. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * (seq_t)10 + (seq_t)cur[i];
    }

    free(cur);
    free(next);
    return result;
}
