/*
 * A319611  a(n) is the number of gaps in the n-th generation of the rule-30
 *          1D cellular automaton started from a single ON.
 * https://oeis.org/A319611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   simulate the Rule‑30 cellular automaton started from a single ON
 *            cell; return the number of gaps (runs of consecutive OFF cells
 *            bounded by ON cells) in generation n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n may be up to about 10 000; we use a bit–array of width 2*n+1
       (indices 0 … 2*n).  */
    size_t N = (size_t)n;
    size_t max_cells = 2 * N + 1;
    size_t words = (max_cells + 63) >> 6;            /* number of 64‑bit words */

    uint64_t *cur   = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *left  = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *right = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *next  = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!cur || !left || !right || !next) {
        free(cur); free(left); free(right); free(next);
        return (seq_t)0;
    }

    /* initial single ON cell occupies index N (middle of the array) */
    size_t offset = N;
    cur[offset >> 6] |= (1ULL << (offset & 63));

    /* current active range (inclusive) */
    size_t start = offset;
    size_t end   = offset;

    for (size_t gen = 0; gen <= N; ++gen) {
        /* count gaps when we reach the required generation */
        if (gen == N) {
            int gaps = 0;
            bool prev_one = false;
            bool in_zero_run = false;

            for (size_t idx = start; idx <= end; ++idx) {
                bool one = (cur[idx >> 6] >> (idx & 63)) & 1ULL;
                if (one) {
                    if (in_zero_run) { ++gaps; in_zero_run = false; }
                    prev_one = true;
                } else {
                    if (prev_one && !in_zero_run) { in_zero_run = true; }
                }
            }

            free(cur); free(left); free(right); free(next);
            return (seq_t)gaps;
        }

        /* shift left: bits move toward higher indices */
        uint64_t carry = 0;
        for (size_t i = 0; i < words; ++i) {
            uint64_t w = cur[i];
            left[i] = (w << 1) | carry;
            carry   = w >> 63;
        }

        /* shift right: bits move toward lower indices */
        carry = 0;
        for (size_t i = words; i-- > 0; ) {
            uint64_t w = cur[i];
            right[i] = (w >> 1) | (carry << 63);
            carry    = w & 1ULL;
        }

        /* new row: next = left XOR (cur OR right)  (Rule‑30) */
        for (size_t i = 0; i < words; ++i) {
            next[i] = left[i] ^ (cur[i] | right[i]);
        }

        /* swap buffers for next iteration */
        uint64_t *tmp = cur;
        cur = next;
        next = tmp;
        /* 'next' will be overwritten in the next cycle */

        /* the active range expands by one cell on each side */
        --start;
        ++end;
    }

    /* should never reach here */
    free(cur); free(left); free(right); free(next);
    return (seq_t)0;
}
