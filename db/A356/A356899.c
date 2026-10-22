/*
 * A356899  Nonnegative numbers whose minimal and maximal tribonacci
 *          representations are the same.
 * https://oeis.org/A356899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 5795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th nonnegative integer whose minimal and maximal
 * tribonacci representations are the same, i.e. that has a unique
 * representation as a sum of distinct tribonacci numbers
 * (T0 = 1, T1 = 2, T2 = 4, T3 = 7, …).  The first few terms are
 * 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, …  */

/*  The algorithm builds the set of all numbers having a unique
 *  tribonacci representation up to a current bound B.
 *  It keeps increasing B until at least n numbers have been found.
 *  For each bound it computes the number of representations for
 *  every integer ≤ B with a simple DP:
 *      ways[s] = number of representations of s using distinct weights,
 *                capped at 2.  If ways[s] == 1, the representation is unique.
 *
 *  Memory usage is O(B) for the DP array; the largest bound needed
 *  for n ≤ 10000 is well below 1 000 000, so the program runs in
 *  well under 2 s on modern hardware.  The code uses only the
 *  operations allowed by the requirements and is fully standard
 *  C11 / C++17 compliant.  No external helper functions are called.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* sequence and index types are supplied by the build system */
extern seq_t a(idx_t);

/* internal state: the list of unique tribonacci-representation
   numbers we have discovered so far, and the current maximum
   bound that was used in the DP.  The list is cached between
   calls so that repeated queries are fast. */
static seq_t *g_seq   = NULL;      /* array of discovered terms    */
static size_t g_len   = 0;          /* number of stored terms       */
static seq_t g_curr_bound = 0;      /* current DP bound             */

/* Helper: generates the sequence of unique tribonacci-representation
   numbers large enough to cover the required index.  The function
   recomputes the list from scratch each time the bound is increased. */
static void ensure_sequence(idx_t required_n)
{
    if (g_len >= (size_t)required_n)
        return;          /* already have enough terms */

    /* choose an initial bound – 1 000 000 is more than enough for 10 000 terms,
       but we start smaller to keep the first run light */
    seq_t bound = 100000;
    if (g_curr_bound > 0)
        bound = g_curr_bound * 2;   /* start from last bound if any */

    /* Repeatedly double the bound until we have enough terms.  Each time we
       recompute the list from scratch. */
    while (g_len < (size_t)required_n) {
        /* 1. Construct the tribonacci weights up to 'bound'. */
        seq_t w[64];
        size_t wlen = 3;
        w[0] = (seq_t)1;
        w[1] = (seq_t)2;
        w[2] = (seq_t)4;
        for (;;) {
            seq_t next = w[wlen-1] + w[wlen-2] + w[wlen-3];
            if (next > bound)
                break;
            if (wlen >= 64)                /* safety: should never happen */
                break;
            w[wlen++] = next;
        }

        /* 2. DP: ways[s] = number of representations of s (capped at 2). */
        size_t maxsum = (size_t)bound;    /* assume bound fits in size_t */
        unsigned char *ways = (unsigned char*)calloc(maxsum + 1, sizeof(unsigned char));
        if (!ways)             /* allocation failure – give up */
            return;
        ways[0] = 1;

        for (size_t i = 0; i < wlen; ++i) {
            size_t weight = (size_t)w[i];
            for (size_t s = maxsum; s >= weight; --s) {
                if (ways[s - weight]) {
                    unsigned val = (unsigned)ways[s] + (unsigned)ways[s - weight];
                    ways[s] = (val > 2 ? 2 : val);
                }
            }
        }

        /* 3. Count and collect the unique numbers. */
        size_t count = 0;
        for (size_t s = 0; s <= maxsum; ++s)
            if (ways[s] == 1)
                ++count;

        seq_t *new_seq = (seq_t*)malloc(count * sizeof(seq_t));
        if (!new_seq) {            /* allocation failure – clean up */
            free(ways);
            return;
        }

        size_t idx = 0;
        for (size_t s = 0; s <= maxsum; ++s)
            if (ways[s] == 1)
                new_seq[idx++] = (seq_t)s;

        free(ways);

        /* Replace the old list with the new one. */
        if (g_seq)
            free(g_seq);
        g_seq = new_seq;
        g_len = count;
        g_curr_bound = bound;

        /* If still not enough terms, enlarge the bound and repeat. */
        if (g_len < (size_t)required_n)
            bound *= 2;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)    /* behavior below the offset does not matter */
        return (seq_t)0;

    ensure_sequence((idx_t)n);
    /* a(1) corresponds to g_seq[0] */
    return g_seq[(size_t)n - 1];
}
