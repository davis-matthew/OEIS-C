/*
 * A328102  Column immediately left of the middle column of rule-30 1-D
 *          cellular automaton, when started from a lone 1 cell.
 * https://oeis.org/A328102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9888, u16 n <= 9888, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A328102: Column immediately left of the middle column of
   rule‑30 1‑D cellular automaton, starting from a single 1.
   The function simulates the CA for n steps and returns the
   bit at position center‑1 (the left neighbour of the middle). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* for n < 1 the offset is irrelevant; return 0 */
    if (n == 0) return (seq_t)0;

    /* number of steps and width of the row after the last step */
    size_t steps = (size_t)n;
    size_t bits  = 2 * steps + 1;
    size_t words = (bits + 63) >> 6;           /* 64‑bit words per row */

    /* dynamic arrays for rows and temporary shifted copies */
    uint64_t *prev  = (uint64_t*)calloc(words, sizeof(uint64_t));
    uint64_t *curr  = (uint64_t*)calloc(words, sizeof(uint64_t));
    uint64_t *left  = (uint64_t*)calloc(words, sizeof(uint64_t));
    uint64_t *right = (uint64_t*)calloc(words, sizeof(uint64_t));

    if (!prev || !curr || !left || !right) {
        /* allocation failed – return 0 (the sequence is 0/1 anyway) */
        free(prev); free(curr); free(left); free(right);
        return (seq_t)0;
    }

    /* position of the initial single 1 (the centre) in the bit array */
    size_t center = steps;
    prev[center >> 6] |= 1ULL << (center & 63);

    /* mask to clear bits beyond the valid width after each step */
    uint64_t mask_last = (bits & 63) ? ((1ULL << (bits & 63)) - 1ULL) : ~0ULL;

    /* iterate the CA */
    for (size_t s = 1; s <= steps; ++s) {
        /* left shift:  prev << 1 */
        uint64_t carry = 0;
        for (size_t i = 0; i < words; ++i) {
            uint64_t w = prev[i];
            left[i] = (w << 1) | carry;
            carry   = w >> 63;
        }

        /* right shift: prev >> 1 */
        carry = 0;
        for (size_t i = words; i-- > 0; ) {
            uint64_t w = prev[i];
            right[i] = (w >> 1) | (carry << 63);
            carry    = w & 1ULL;
        }

        /* rule‑30: new[i] = left[i] XOR (prev[i] OR right[i]) */
        for (size_t i = 0; i < words; ++i) {
            curr[i] = left[i] ^ (prev[i] | right[i]);
        }
        curr[words - 1] &= mask_last;          /* keep only valid bits */

        /* swap for next step */
        uint64_t *tmp = prev; prev = curr; curr = tmp;
    }

    /* extract the bit at position center-1 (left of the middle) */
    uint64_t bit = (prev[(steps - 1) >> 6] >> ((steps - 1) & 63)) & 1ULL;

    /* free temporary buffers */
    free(prev); free(curr); free(left); free(right);

    return (seq_t)bit;
}
