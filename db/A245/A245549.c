/*
 * A245549  State of one-dimensional cellular automaton 'sigma' (Rule 30):
 *          000,001,010,011,100,101,110,111 -> 0,0,0,1,1,1,1,0 at generation
 *          n, regarded as a binary number.
 * https://oeis.org/A245549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the binary configuration of one‑dimensional * cellular
 *            automaton 'sigma' (Rule 30) after n generations, * starting from
 *            a single ON cell. The cells from −n to +n are * concatenated
 *            left‑to‑right; the resulting binary string is then * interpreted
 *            as a decimal integer with digits 0 and 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Rule 30 mapping: pattern (left<<2)|(center<<1)|right -> next state.
 * Patterns enumerate 000,001,010,011,100,101,110,111. */
static const unsigned char rule30[8] = {
    0, /* 000 */
    1, /* 001 */
    1, /* 010 */
    1, /* 011 */
    1, /* 100 */
    0, /* 101 */
    0, /* 110 */
    0  /* 111 */
};

seq_t a(idx_t n)
{
    /* Generation 0: single ON cell at position 0. */
    if (n == 0)
        return 1;

    /* Length of the buffer needed to hold all cells that may be set
     * during n generations: indices from −(n+1) to +(n+1).          */
    const size_t len    = 2 * (size_t)n + 3;
    const size_t offset = (size_t)n + 1;        /* map cell 0 to array index 'offset'. */

    /* Allocate two buffers and zero them. */
    unsigned char *buf1 = (unsigned char*)calloc(len, sizeof(unsigned char));
    unsigned char *buf2 = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (!buf1 || !buf2) {
        free(buf1);
        free(buf2);
        return 0;   /* on allocation failure return 0. */
    }

    /* Initialise generation 0 (single ON cell). */
    buf1[offset] = 1;

    unsigned char *cur  = buf1;
    unsigned char *next = buf2;

    for (idx_t gen = 1; gen <= n; ++gen) {
        const size_t lo = offset - (size_t)gen;
        const size_t hi = offset + (size_t)gen;

        /* Compute next state for cells in the current support. */
        for (size_t i = lo; i <= hi; ++i) {
            unsigned char left   = cur[i - 1];
            unsigned char center = cur[i];
            unsigned char right  = cur[i + 1];
            unsigned char pat    = (left << 2) | (center << 1) | right;
            next[i] = rule30[pat];
        }

        /* Explicitly clear the cells just outside the new support. */
        next[lo - 1] = 0;
        next[hi + 1] = 0;

        /* Swap buffers for the next iteration. */
        unsigned char *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Convert the final binary string (cells −n … +n) to a decimal
     * integer whose digits are 0 and 1. */
    seq_t result = 0;
    for (size_t i = offset - (size_t)n; i <= offset + (size_t)n; ++i) {
        result = result * 10 + cur[i];
    }

    free(buf1);
    free(buf2);
    return result;
}
