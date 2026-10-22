/*
 * A379397  Numbers that can be written in exactly two different ways as a sum
 *          of at most nine positive third powers.
 * https://oeis.org/A379397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   alloc_or_realloc, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by enumerating sums of at most nine positive cubes. */
/* The function memoises the list of numbers with exactly two distinct
   representations and grows the search limit when more terms are needed. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>
#include "../utils/int_cube_root.h"
#include "../utils/alloc_or_realloc.h"

static void compute_up_to(idx_t limit, seq_t **out_seq, idx_t *out_len)
{
    /* dp[k*sz + s] counts ways to represent s using exactly k cubes, capped at 3 */
    size_t sz = (size_t)(limit + 1);
    uint8_t *dp = (uint8_t*)calloc(10 * sz, sizeof(uint8_t));
    if (!dp) { *out_seq = NULL; *out_len = 0; return; }
    dp[0] = 1; /* empty representation */

    idx_t max_base = int_cube_root(limit);
    for (idx_t base = 1; base <= max_base; ++base) {
        idx_t cube = (idx_t)base * base * base;
        for (int k = 1; k <= 9; ++k) {
            size_t k_off = (size_t)k * sz;
            size_t km1_off = (size_t)(k - 1) * sz;
            for (idx_t s = cube; s <= limit; ++s) {
                uint8_t prev = dp[km1_off + (size_t)(s - cube)];
                if (!prev) continue;
                uint8_t *cur = &dp[k_off + (size_t)s];
                uint8_t val = *cur + prev;
                if (val > 2) val = 3;
                *cur = val;
            }
        }
    }

    /* collect numbers with total count == 2 */
    size_t cap = 0;
    size_t count = 0;
    seq_t *seq = NULL;
    for (idx_t s = 1; s <= limit; ++s) {
        uint8_t total = 0;
        for (int k = 1; k <= 9; ++k) {
            total += dp[(size_t)k * sz + (size_t)s];
            if (total > 2) { total = 3; break; }
        }
        if (total == 2) {
            seq = (seq_t*)alloc_or_realloc(seq, &cap, count + 1, sizeof(seq_t));
            seq[count++] = (seq_t)s;
        }
    }
    free(dp);
    *out_seq = seq;
    *out_len = (idx_t)count;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *sequence = NULL;
    static idx_t seq_len = 0;
    static idx_t max_limit = 200000;          /* initial search bound */

    if (!initialized) {
        compute_up_to(max_limit, &sequence, &seq_len);
        initialized = true;
    }

    while (n > seq_len) {
        /* enlarge search limit and recompute */
        max_limit *= 2;
        seq_t *new_seq = NULL;
        idx_t new_len = 0;
        compute_up_to(max_limit, &new_seq, &new_len);
        if (sequence) free(sequence);
        sequence = new_seq;
        seq_len = new_len;
    }

    return sequence[n - 1];   /* a(1) -> sequence[0] */
}
