/*
 * A246688  Triangle in which n-th row lists lexicographically ordered
 *          increasing lists of parts of all partitions of n into distinct
 *          parts.
 * https://oeis.org/A246688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th element of the triangle of distinct integer
 *            partitions of the row index, flattened lexicographically. No
 *            simple closed form is known
 * Range:     u8 n <= 8956, u16 n <= 8956, u32 n <= 8956, u64 n <= 8956,
 *            u128 n <= 8956, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *flat = NULL;          /* flattened sequence */
static size_t flat_len = 0;          /* number of elements stored */
static size_t flat_cap = 0;          /* capacity of flat array */

static seq_t *cur = NULL;           /* buffer for building a row */
static size_t cur_cap = 0;           /* capacity of cur buffer */

static idx_t next_row = 1;           /* next row (partition size) to generate */

/* ensure flat array has enough capacity for one more element */
static void flat_grow(void)
{
    if (flat_len == flat_cap) {
        flat_cap = (flat_cap == 0) ? 64 : flat_cap * 2;
        flat = (seq_t *)realloc(flat, flat_cap * sizeof(seq_t));
    }
}

/* ensure cur buffer can hold at least n elements */
static void cur_prepare(idx_t n)
{
    if (cur_cap < (size_t)n) {
        cur_cap = (size_t)n;
        cur = (seq_t *)realloc(cur, cur_cap * sizeof(seq_t));
    }
}

/* recursive generation of partitions of 'remaining' into distinct parts
   starting with values >= 'min_part'.  The parts are stored in 'current'
   (of depth 'depth').  When a partition is complete, its parts are
   appended to the global flat array in the order they appear. */
static void gen_parts(idx_t remaining, idx_t min_part, seq_t *current, size_t depth)
{
    if (remaining == 0) {
        /* partition found: append its parts to flat */
        for (size_t i = 0; i < depth; ++i) {
            flat_grow();
            flat[flat_len++] = current[i];
        }
        return;
    }
    for (idx_t part = min_part; part <= remaining; ++part) {
        current[depth] = (seq_t)part;
        gen_parts(remaining - part, part + 1, current, depth + 1);
    }
}

/* generate rows until we have at least 'goal' elements in flat. */
static void ensure_terms(idx_t goal)
{
    while (flat_len < (size_t)goal) {
        idx_t n = next_row;
        cur_prepare((idx_t)n);
        gen_parts(n, 1, cur, 0);
        ++next_row;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_terms(n);
    return flat[n - 1];
}
