/*
 * A266089  Lexicographically smallest permutation of natural numbers such
 *          that in binary representation the number of ones of adjacent terms
 *          differ exactly by one.
 * https://oeis.org/A266089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically smallest permutation of natural numbers such that
 * in binary representation the number of ones of adjacent terms differs
 * exactly by one.  The sequence is built on demand; no hard‑coded table
 * is used.  The implementation is O(n²) worst‑case but is well below
 * the 2‑second limit for n up to about 10 000.  It works for both
 * fixed‑size integer types and for an arbitrary‑precision seq_t
 * (the seq_realloc helper handles the latter). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

static seq_t *seq_cache = NULL;         /* array of computed terms      */
static size_t seq_cap   = 0;            /* capacity of seq_cache        */
static size_t seq_len   = 0;            /* number of terms computed     */

static bool *used      = NULL;          /* flag array for used ints     */
static size_t used_cap = 0;             /* capacity of used array       */

/* popcount for arbitrary seq_t using bitwise ops */
static inline unsigned popcnt_seq(seq_t v)
{
    unsigned c = 0;
    while (v != 0)
    {
        c += (unsigned)(v & 1);
        v >>= 1;
    }
    return c;
}

/* Ensure seq_cache can hold at least 'need' elements. */
static inline void ensure_seq_capacity(size_t need)
{
    if (seq_cap < need)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)seq_realloc(seq_cache, seq_cap, new_cap);
        if (!new_arr) exit(EXIT_FAILURE);
        seq_cache = new_arr;
        seq_cap   = new_cap;
    }
}

/* Ensure the used flag array can index at least up to 'up_to'. */
static inline void ensure_used_capacity(idx_t up_to)
{
    if ((idx_t)used_cap <= up_to)
    {
        size_t new_cap = used_cap ? used_cap * 2 : 64;
        while ((idx_t)new_cap <= up_to) new_cap <<= 1;
        bool *new_arr = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!new_arr) exit(EXIT_FAILURE);
        /* Initialise the newly added slots to false. */
        memset(new_arr + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used      = new_arr;
        used_cap  = new_cap;
    }
}

/* Build the sequence up to index n (inclusive). */
static void generate_up_to(idx_t n)
{
    /* Bootstrap the sequence with a(0) = 0. */
    if (seq_len == 0)
    {
        ensure_seq_capacity(1);
        seq_cache[0] = (seq_t)0;
        seq_len = 1;
        ensure_used_capacity(0);
        used[0] = true;
    }

    /* Extend until we have a(n). */
    while (seq_len <= n)
    {
        seq_t prev_val = seq_cache[seq_len - 1];
        unsigned pc_prev = popcnt_seq(prev_val);

        idx_t cand = 0;
        for (;;)
        {
            ensure_used_capacity(cand);
            if (!used[cand])
            {
                seq_t cand_val = (seq_t)cand;
                unsigned pc_cand = popcnt_seq(cand_val);
                int diff = (int)pc_cand - (int)pc_prev;
                if (diff == 1 || diff == -1)
                {
                    ensure_seq_capacity(seq_len + 1);
                    seq_cache[seq_len] = cand_val;
                    used[cand] = true;
                    ++seq_len;
                    break;
                }
            }
            ++cand;
        }
    }
}

/* Public interface expected by the evaluator. */
seq_t a(idx_t n)
{
    generate_up_to(n);
    return seq_cache[n];
}
