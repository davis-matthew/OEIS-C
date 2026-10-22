/*
 * A326109  Lexicographically earliest sequence of distinct terms such that
 *          a(n) is divisible by three and only three digits of a(n+1).
 * https://oeis.org/A326109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 2108, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326109: Lexicographically earliest sequence of distinct terms
   such that a(n) is divisible by exactly three decimal digits of a(n+1).
   a(1) = 1.  This implementation computes each term sequentially and
   memoizes the results; it does not assume that calls to a() arrive
   in increasing order. */

/* Note: The types seq_t and idx_t are supplied by the build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Dynamic array holding the computed terms. */
static seq_t *seq = NULL;
static size_t seq_cap = 0;   /* allocated number of elements */
static size_t seq_len = 0;   /* number of terms stored */

/* Growth factor for the sequence array. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < need) {
        new_cap <<= 1;
    }
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Linear search to determine if a value has already appeared. */
static bool seen_seq(const seq_t v)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == v)
            return true;
    return false;
}

/* For each possible mask of digits that divide the previous term,
   keep the next search start. 512 masks (2^9). */
static seq_t start_arr[512];
static bool start_flag[512] = {false};

/* Find the lexicographically earliest unused integer that has
   exactly three digits which divide the given previous term. */
static seq_t find_next(const seq_t prev)
{
    /* Build a bitmask of digits (1-9) that divide prev. */
    unsigned mask = 0;
    for (int d = 1; d <= 9; ++d) {
        if (prev % (seq_t)d == 0)
            mask |= 1u << (d - 1);
    }

    /* Initialise the start position for this mask if needed. */
    if (!start_flag[mask]) {
        start_arr[mask] = 1;
        start_flag[mask] = true;
    }

    seq_t cand = start_arr[mask];
    for (;;) {
        /* Skip values already used in the sequence. */
        if (!seen_seq(cand)) {
            /* Count how many digits of cand belong to the mask. */
            seq_t t = cand;
            int count = 0;
            while (t > 0) {
                int d = (int)(t % 10);
                t /= 10;
                if (d > 0 && ((mask >> (d - 1)) & 1u)) {
                    ++count;
                    if (count > 3) break;
                }
            }
            if (count == 3) {
                /* Advance the start position for the next use of this mask. */
                start_arr[mask] = cand + 1;
                return cand;
            }
        }
        ++cand;
    }
}

/* Definition of the sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */

    /* Initialise the first term once. */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = (idx_t)seq_len + 1; i <= n; ++i) {
        seq_t prev = seq[seq_len - 1];
        seq_t next = find_next(prev);
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = next;
        ++seq_len;
    }

    return seq[n - 1];
}
