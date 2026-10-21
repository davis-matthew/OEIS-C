/*
 * A265281  Decimal representation of the n-th iteration of the "Rule 86"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = decimal representation of the n‑th iteration of the “Rule
 *            86” elementary cellular automaton starting with a single ON cell
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial state: one ON cell */
    size_t cur_len = 1;
    uint8_t *curr = (uint8_t *)malloc(cur_len);
    if (!curr) return (seq_t)0;   /* unlikely but defensive */
    curr[0] = 1;

    /* iterate the cellular automaton */
    for (idx_t step = 1; step <= n; ++step) {
        size_t new_len = cur_len + 2;
        uint8_t *next = (uint8_t *)malloc(new_len);
        if (!next) { free(curr); return (seq_t)0; }

        for (size_t j = 0; j < new_len; ++j) {
            uint8_t left  = (j >= 2 && (j - 2) < cur_len) ? curr[j - 2] : 0;
            uint8_t self  = (j >= 1 && (j - 1) < cur_len) ? curr[j - 1] : 0;
            uint8_t right = (j < cur_len) ? curr[j] : 0;
            /* Rule 86: output 1 for the patterns 110, 100, 010, 001 */
            uint8_t bit = (left && self && !right)   ||
                          (left && !self && !right)  ||
                          (!left && self && !right)  ||
                          (!left && !self && right);
            next[j] = bit;
        }

        free(curr);
        curr = next;
        cur_len = new_len;
    }

    /* convert the final binary pattern to its decimal value */
    seq_t value = 0;
    for (size_t i = 0; i < cur_len; ++i) {
        value = (value << 1) + curr[i];
    }

    free(curr);
    return value;
}
