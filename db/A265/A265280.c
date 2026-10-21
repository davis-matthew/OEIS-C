/*
 * A265280  Binary representation of the n-th iteration of the "Rule 86"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the integer whose decimal digits are the bits of the *
 *            nth iteration of Rule 86 elementary cellular automaton starting
 *            from a * single ON cell
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Rule 86: mapping from 3‑bit neighbourhood to new state.
   index = (left<<2)|(mid<<1)|right */
static const unsigned char rule86_map[8] = {0,1,1,0,1,0,1,0};

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* a(0) = "1" */

    size_t N = (size_t)n;
    /* array size: width 2*N+1 plus a 0 border on each side */
    size_t size   = 2 * N + 3;
    size_t offset = N + 1;          /* centre position of the initial cell */

    unsigned char *prev = (unsigned char*)calloc(size, 1);
    unsigned char *next = (unsigned char*)calloc(size, 1);
    if (!prev || !next) {
        free(prev);
        free(next);
        return 0;
    }

    prev[offset] = 1;               /* initial single ON cell */

    unsigned char *cur = prev, *nxt = next;

    for (size_t step = 0; step < N; ++step) {
        size_t start = offset - (step + 1);
        size_t end   = offset + (step + 1);
        for (size_t k = start; k <= end; ++k) {
            unsigned left  = cur[k - 1];
            unsigned mid   = cur[k];
            unsigned right = cur[k + 1];
            unsigned idx   = (left << 2) | (mid << 1) | right;
            nxt[k] = rule86_map[idx];
        }
        unsigned char *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Build the decimal value whose digits are the final row bits. */
    seq_t val = 0;
    size_t row_start = offset - N;
    size_t row_end   = offset + N;
    for (size_t k = row_start; k <= row_end; ++k) {
        val = val * 10 + cur[k];
    }

    free(prev);
    free(next);
    return val;
}
