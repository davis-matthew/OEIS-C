/*
 * A287411  Start with 0 and repeatedly substitute 0->012, 1->120, 2->021.
 * https://oeis.org/A287411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: fixed point of the morphism 0→012, 1→120, 2→021
   starting with the symbol 0.  The function returns the n‑th
   symbol (1‑indexed) in this infinite word.  */
   
#include <stddef.h>  /* for size_t */

static const seq_t mapping[3][3] = {
    {0, 1, 2},   /* 0 → 012 */
    {1, 2, 0},   /* 1 → 120 */
    {0, 2, 1}    /* 2 → 021 */
};

seq_t a(idx_t n) {
    /* We compute the sequence symbol iteratively, climbing up the
       substitution tree until we reach the start symbol 0, then
       unwind the stack applying the morphism.  */
    uint8_t stack[64];          /* enough for n up to 2^64-1 (log_3 ~ 40) */
    size_t top = 0;

    /* climb up to the root (position 1) */
    while (n > 1) {
        uint8_t offset = (uint8_t)((n - 1) % 3U);
        stack[top++] = offset;
        n = (n - 1) / 3U + 1U;
    }

    seq_t value = 0;  /* start symbol */
    while (top) {
        uint8_t offset = stack[--top];
        value = mapping[(unsigned)value][offset];
    }
    return value;
}
