/*
 * A267577  Binary representation of the n-th iteration of the "Rule 167"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binary representation of the n-th iteration of elementary
 *            cellular automaton Rule 167 starting from a single ON cell at
 *            the origin. The result is the decimal integer whose decimal
 *            digits are the binary digits of the automaton state
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;                       /* a(0) = 1 */

    /* width of the working array: 4*n + 1 is sufficient because
       cells a distance ≤ 2*n from the origin can influence the
       interior window [ -n .. n ] after n steps. */
    size_t L     = 4 * (size_t)n + 1;
    size_t off   = 2 * (size_t)n;              /* index of the origin */

    /* Current and next state arrays.  Values are 0 or 1. */
    uint8_t *cur  = (uint8_t*)malloc(L);
    uint8_t *next = (uint8_t*)malloc(L);
    if (!cur || !next) {
        free(cur);
        free(next);
        return 0; /* allocation failure, return 0 */
    }

    memset(cur, 0, L);
    cur[off] = 1;                              /* initial cell */

    /* Rule 167 mapping: pattern abc (a=left, b=center, c=right)
       -> new bit.  Index = a*4 + b*2 + c. */
    static const uint8_t rule167[8] = { 1, 1, 1, 0, 0, 1, 0, 1 };

    /* Iterate n steps */
    for (idx_t step = 0; step < n; ++step) {
        for (size_t i = 0; i < L; ++i) {
            uint8_t left  = (i > 0)     ? cur[i - 1] : 0;
            uint8_t mid   = cur[i];
            uint8_t right = (i + 1 < L) ? cur[i + 1] : 0;
            uint8_t p     = (left << 2) | (mid << 1) | right;
            next[i] = rule167[p];
        }
        /* swap cur and next */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Build the decimal integer from the bits in the window [-n .. n] */
    seq_t result = 0;
    for (idx_t i = off - n; i <= off + n; ++i) {
        result = result * 10 + (seq_t)(cur[i]);
    }

    free(cur);
    free(next);
    return result;
}
