/*
 * A267356  Binary representation of the n-th iteration of the "Rule 124"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the binary representation of the n‑th iteration of the
 *            Rule 124 elementary cellular automaton starting with a single ON
 *            cell
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    /* Rule 124 truth table:
       pattern bits abc (a = left neighbor, b = current cell, c = right neighbor)
       output 1 for all patterns except 111, 001, 000. */
    static const uint8_t rule_map[8] = {
        0, /* 000 */
        0, /* 001 */
        1, /* 010 */
        1, /* 011 */
        1, /* 100 */
        1, /* 101 */
        1, /* 110 */
        0  /* 111 */
    };

    /* The longest possible row after n steps has length n+1 bits. */
    size_t max_len = (size_t)n + 1;

    /* Allocate two temporary rows and zero them. */
    uint8_t *row1 = (uint8_t *)calloc(max_len + 1, sizeof(uint8_t));
    uint8_t *row2 = (uint8_t *)calloc(max_len + 1, sizeof(uint8_t));
    if (!row1 || !row2) {
        /* Allocation failed – return zero. */
        seq_t zero = 0;
        free(row1);
        free(row2);
        return zero;
    }

    /* Initial configuration: single ON cell. */
    row1[0] = 1;
    size_t cur_len = 1;

    uint8_t *cur  = row1;
    uint8_t *next = row2;

    /* Perform n iterations of Rule 124. */
    for (idx_t step = 0; step < n; ++step) {
        size_t new_len = cur_len + 1;

        for (size_t i = 0; i < new_len; ++i) {
            uint8_t left  = (i > 0)          ? cur[i - 1] : 0;
            uint8_t mid   = (i < cur_len)    ? cur[i]     : 0;
            uint8_t right = (i + 1 < cur_len)? cur[i + 1] : 0;
            uint8_t pat   = (left << 2) | (mid << 1) | right;
            next[i] = rule_map[pat];
        }

        /* Swap current and next rows for the next iteration. */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
        cur_len = new_len;
    }

    /* Convert the binary row into a decimal string consisting of the bits. */
    char *buf = (char *)malloc(cur_len + 1);
    if (!buf) {
        free(row1);
        free(row2);
        seq_t zero = 0;
        return zero;
    }
    for (size_t i = 0; i < cur_len; ++i) {
        buf[i] = cur[i] ? '1' : '0';
    }
    buf[cur_len] = '\0';

    seq_t result = seq_from_str(buf);

    free(buf);
    free(row1);
    free(row2);
    return result;
}
