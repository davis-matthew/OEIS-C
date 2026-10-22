/*
 * A351205  Numbers whose binary expansion does not have all distinct runs.
 * https://oeis.org/A351205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) counts numbers whose binary expansion has at least one
 *            repeated run of identical bits, i.e. the same (bit value, run
 *            length) pair occurs more than once. a(1)=5, the first such
 *            number
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* * Dependencies *
   This code uses the helper function
        unsigned bitlen_seq_t(seq_t x);
   defined in the utils/bitlen_seq_t.h header.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

/* ------------------------------------------------------------------ */
/* Helper: test whether a number has a repeated run in its binary      */
/* representation.                                                     */
/* ------------------------------------------------------------------ */
static bool has_repeated_runs(seq_t x)
{
    if (x == 0)                /* 0 has only one run of length 1 */
        return false;

    /* Determine number of significant bits.  For arbitrary precision
       the returned value may be larger than 64 but at most the bit
       width of seq_t, which is fine for small stacks. */
    unsigned len = bitlen_seq_t(x);
    if (len <= 1)
        return false;          /* single-bit numbers cannot repeat */

    /* Maximum run count equals the number of bits (all alternating bits). */
    /* For 128-bit seq_t this is 128, which is trivial to hold on stack. */
    const unsigned MAX_RUNS = 128;
    seq_t run_bit[MAX_RUNS];
    unsigned run_len[MAX_RUNS];
    unsigned run_cnt = 0;

    /* Initialise first bit and run length */
    seq_t prev_bit = (x >> (len - 1)) & 1;
    unsigned curr_len = 1;

    /* Scan bits from second most significant down to least significant */
    for (int i = (int)(len - 2); i >= 0; --i) {
        seq_t bit = (x >> i) & 1;
        if (bit == prev_bit) {
            ++curr_len;        /* extend current run */
        } else {
            /* Compare the completed run against all previous runs */
            for (unsigned j = 0; j < run_cnt; ++j) {
                if (run_bit[j] == prev_bit && run_len[j] == curr_len)
                    return true;        /* repeated run found */
            }
            if (run_cnt < MAX_RUNS) {   /* store new run */
                run_bit[run_cnt] = prev_bit;
                run_len[run_cnt] = curr_len;
                ++run_cnt;
            }
            prev_bit = bit;
            curr_len = 1;
        }
    }

    /* Final run check */
    for (unsigned j = 0; j < run_cnt; ++j) {
        if (run_bit[j] == prev_bit && run_len[j] == curr_len)
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main sequence function: a(idx_t n)                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for 0, return 0 as placeholder */
        return (seq_t)0;

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* Next integer to test, stored as idx_t to avoid overflow */
    static idx_t next_candidate = 1;

    /* Build cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Ensure capacity for another element */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();    /* fatal allocation error */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Test candidate (potential overflow for very small seq_t,
           but the sequence is defined only where it fits). */
        seq_t val = (seq_t)next_candidate;
        if (has_repeated_runs(val))
            cache[cache_len++] = val;

        ++next_candidate;
    }

    return cache[n - 1];
}
