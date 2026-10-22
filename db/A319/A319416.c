/*
 * A319416  Cuts-resistance of n: number of applications of Lenormand's
 *          "raboter" map needed to transform the binary expansion of n to the
 *          empty string.
 * https://oeis.org/A319416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9151, u16 n <= 9151, u32 n <= 9151, u64 n <= 9151,
 *            u128 n <= 9151, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of applications of Lenormand's raboter map needed
   to reduce the binary expansion of n to the empty string (OEIS A319416).
   The algorithm repeatedly builds the run–length representation of the
   binary string, decrements each run length, removes zeros, and merges
   adjacent runs of the same bit value that arise after a zero-length run
   is omitted.  The process stops when no runs remain. */
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_log2_uint64.h"

typedef struct { int bit; int len; } Run;

seq_t a(idx_t n)
{
    /* Special case n == 0: binary "0" becomes empty in one step */
    if (n == 0) return (seq_t)1;

    /* Determine number of bits needed to represent n */
    idx_t bits = floor_log2_uint64(n) + 1;

    /* Build initial run representation from most–significant bit */
    Run runs1[65], runs2[65];
    int run_count = 0;
    int bit = (n >> (bits - 1)) & 1;
    int len = 0;
    for (idx_t i = 0; i < bits; ++i) {
        int curbit = (n >> (bits - 1 - i)) & 1;
        if (curbit == bit) { len++; }
        else {
            runs1[run_count].bit = bit;
            runs1[run_count].len = len;
            run_count++;
            bit = curbit;
            len = 1;
        }
    }
    runs1[run_count].bit = bit;
    runs1[run_count].len = len;
    run_count++;

    seq_t steps = 0;
    Run *cur = runs1, *next = runs2, *tmp;

    while (run_count > 0) {
        steps++;

        /* Decrement each run and merge equal‑bit neighbors */
        int next_count = 0;
        for (int i = 0; i < run_count; ++i) {
            int new_len = cur[i].len - 1;
            if (new_len <= 0) continue;          /* drop zero‑length run */

            if (next_count > 0 && cur[i].bit == next[next_count - 1].bit) {
                /* merge with previous run of same bit */
                next[next_count - 1].len += new_len;
            } else {
                next[next_count].bit = cur[i].bit;
                next[next_count].len = new_len;
                next_count++;
            }
        }

        /* Swap current and next arrays */
        tmp = cur;
        cur = next;
        next = tmp;
        run_count = next_count;
    }

    return steps;
}
