/*
 * A337866  Lexicographically earliest sequence of distinct positive integers
 *          such that a(n) + a(n+1) has at least one pair of identical
 *          adjacent digits.
 * https://oeis.org/A337866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the lexicographically earliest sequence of
   distinct positive integers such that for all n>=1,
   a(n)+a(n+1) contains at least one pair of identical adjacent
   decimal digits.  The sequence is built greedily: at step n>1 the
   smallest unused integer c is chosen for a(n) so that c+a(n‑1)
   contains a repeated adjacent pair of digits.  For n=1 the first
   term is 1. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/no_consecutive_equal_digits.h"

/* static state */
static seq_t *seq = NULL;        /* 1‑based array of terms: seq[i] = a(i) */
static size_t seq_cap = 0;      /* allocated length of seq array */
static idx_t seq_len = 0;       /* last computed index */

static bool *used = NULL;        /* bitset of values already used */
static size_t used_cap = 0;      /* allocated size of 'used' */

/* Ensure seq array has space for indices up to need-1 (i.e. need elements). */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure used array can index values up to need-1. Newly allocated slots are false. */
static void ensure_used_cap(size_t need)
{
    if (used_cap < need) {
        size_t new_cap = used_cap ? used_cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
        for (size_t i = used_cap; i < new_cap; ++i) new_used[i] = false;
        used = new_used;
        used_cap = new_cap;
    }
}

/* Test whether a value has already been used. */
static bool is_used(seq_t v)
{
    size_t idx = (size_t)v;
    return idx < used_cap && used[idx];
}

/* Generate the sequence up to index n (inclusive). */
static void build_to(idx_t n)
{
    if (seq_len >= n) return;

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else {
            seq_t prev = seq[i - 1];
            seq_t cand = 1;
            while (true) {
                if (!is_used(cand)) {
                    seq_t sum = cand + prev;
                    if (!no_consecutive_equal_digits(sum)) {
                        val = cand;
                        break;
                    }
                }
                ++cand;
            }
        }

        ensure_seq_cap((size_t)i + 1);
        seq[i] = val;

        if ((size_t)val >= used_cap)
            ensure_used_cap((size_t)val + 1);
        used[(size_t)val] = true;
    }
    seq_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1 */
    build_to(n);
    return seq[(size_t)n];
}
