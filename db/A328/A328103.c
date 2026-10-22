/*
 * A328103  Bitwise XOR of trajectories of rule 30 and rule 124, when both are
 *          started from a lone 1 cell: a(n) = A110240(n) XOR A267357(n).
 * https://oeis.org/A328103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A110240(n) XOR A267357(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the XOR of the integer
   representation of the n‑th row of elementary
   cellular automata rule 30 and rule 124,
   both started from a single 1 cell. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Elementary rule tables: 111→0,110→0,101→0,100→1,011→1,010→1,001→1,000→0
   for rule 30; and
   rule 124: 111→0,110→1,101→1,100→1,011→1,010→1,001→0,000→0. */
static const unsigned rule30[8]  = {0, 1, 1, 1, 1, 0, 0, 0};
static const unsigned rule124[8] = {0, 0, 1, 1, 1, 1, 1, 0};

/* Return the bit at logical position idx (0 = leftmost) of a bit‑array
   stored in 64‑bit words.  idx is bounded by width. */
static inline unsigned get_bit(const uint64_t *arr, size_t width, size_t idx)
{
    if (idx >= width) return 0;
    return (arr[idx >> 6] >> (idx & 63)) & 1;
}

/* Set the bit at logical position idx in a 64‑bit word array. */
static inline void set_bit(uint64_t *arr, size_t idx)
{
    arr[idx >> 6] |= 1ULL << (idx & 63);
}

seq_t a(idx_t n)
{
    /* Width of the nth row = 2*n + 1 cells. */
    const size_t width = (size_t)((seq_t)2 * n + 1);
    const size_t words = (width + 63) >> 6;   /* number of 64‑bit words needed */

    /* Allocate temporary word arrays (C++ requires explicit cast from void*). */
    uint64_t *bits30 = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *bits124 = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *cur = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *next = (uint64_t *)calloc(words, sizeof(uint64_t));

    if (!bits30 || !bits124 || !cur || !next) {
        free(bits30); free(bits124); free(cur); free(next);
        return (seq_t)0;       /* allocation failure – safe fallback */
    }

    /* --- Simulate Rule 30 --- */
    cur[0] = 1ULL;            /* initial single 1 at the leftmost position */
    size_t cur_width = 1;

    for (idx_t step = 1; step <= n; ++step) {
        const size_t new_width = cur_width + 2;
        memset(next, 0, words * sizeof(uint64_t));

        for (size_t j = 0; j < new_width; ++j) {
            unsigned left   = (j >= 2)   ? get_bit(cur, cur_width, j - 2) : 0;
            unsigned center = (j >= 1)   ? get_bit(cur, cur_width, j - 1) : 0;
            unsigned right  = (j < cur_width) ? get_bit(cur, cur_width, j) : 0;
            unsigned pat = (left << 2) | (center << 1) | right;

            if (rule30[pat]) set_bit(next, j);
        }
        /* Advance to the next row */
        uint64_t *tmp = cur; cur = next; next = tmp;
        cur_width = new_width;
    }
    memcpy(bits30, cur, words * sizeof(uint64_t));

    /* --- Simulate Rule 124 --- */
    memset(cur, 0, words * sizeof(uint64_t));
    cur[0] = 1ULL;
    cur_width = 1;

    for (idx_t step = 1; step <= n; ++step) {
        const size_t new_width = cur_width + 2;
        memset(next, 0, words * sizeof(uint64_t));

        for (size_t j = 0; j < new_width; ++j) {
            unsigned left   = (j >= 2)   ? get_bit(cur, cur_width, j - 2) : 0;
            unsigned center = (j >= 1)   ? get_bit(cur, cur_width, j - 1) : 0;
            unsigned right  = (j < cur_width) ? get_bit(cur, cur_width, j) : 0;
            unsigned pat = (left << 2) | (center << 1) | right;

            if (rule124[pat]) set_bit(next, j);
        }
        uint64_t *tmp = cur; cur = next; next = tmp;
        cur_width = new_width;
    }
    memcpy(bits124, cur, words * sizeof(uint64_t));

    free(cur); free(next);   /* temporary rows no longer needed */

    /* --- Compute XOR and convert to seq_t --- */
    seq_t result = 0;
    for (size_t idx = 0; idx < width; ++idx) {
        unsigned bit = ((bits30[idx >> 6] ^ bits124[idx >> 6]) >> (idx & 63)) & 1;
        result = (result << 1) | (seq_t)bit;
    }

    free(bits30);
    free(bits124);
    return result;
}
