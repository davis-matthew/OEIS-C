/*
 * A338618  Lexicographically earliest sequence of distinct positive integers
 *          such that three consecutive terms are never pairwise coprime.
 * https://oeis.org/A338618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of distinct positive integers
   such that any three consecutive terms are not pairwise coprime.
   The sequence is generated greedily: at each step the smallest
   unused integer that keeps the last three terms satisfying
   the condition is chosen. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Ensure that the used array can index up to 'need'. */
static void ensure_used_cap(idx_t need, uint8_t **used, idx_t *used_cap)
{
    while (*used_cap <= need) {
        idx_t new_cap = (*used_cap) ? (*used_cap) * 2 : 256;
        uint8_t *tmp = (uint8_t *)realloc(*used, new_cap * sizeof(uint8_t));
        if (!tmp) return;                    /* allocation failure */
        /* Zero the newly added part */
        memset(tmp + *used_cap, 0, (new_cap - *used_cap) * sizeof(uint8_t));
        *used = tmp;
        *used_cap = new_cap;
    }
}

/* Ensure that the sequence array can store at least 'need' elements. */
static void ensure_seq_cap(idx_t need, seq_t **seq, size_t *seq_cap)
{
    while (*seq_cap <= need) {
        idx_t new_cap = (*seq_cap) ? (*seq_cap) * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(*seq, new_cap * sizeof(seq_t));
        if (!tmp) return;                    /* allocation failure */
        *seq = tmp;
        *seq_cap = (size_t)new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static state: sequence and used flags */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_len = 0;

    static uint8_t *used = NULL;
    static idx_t used_cap = 0;

    if (n == 0) return 0;                  /* undefined, but safe */

    /* Initialize base terms if not yet done */
    if (seq_len == 0) {
        ensure_seq_cap(2, &seq, &seq_cap);
        ensure_used_cap(2, &used, &used_cap);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        used[1] = 1;
        used[2] = 1;
        seq_len = 2;
    }

    /* If requested term already computed, return it */
    if (seq_len >= n) return seq[n - 1];

    /* Generate terms up to n */
    for (idx_t i = seq_len; i < n; ++i) {
        seq_t prev1 = seq[i - 2];   /* a(i-1)  */
        seq_t prev2 = seq[i - 1];   /* a(i)    */

        idx_t cand;
        seq_t chosen;

        for (cand = 1; ; ++cand) {
            /* Ensure used array can index cand */
            ensure_used_cap(cand, &used, &used_cap);
            if (!used[(size_t)cand]) {
                /* If the two previous terms already share a factor,
                   any unused candidate works. */
                if (gcd(prev1, prev2) != (seq_t)1) {
                    chosen = (seq_t)cand;
                    break;
                }
                /* Otherwise need candidate to share a factor with at least
                   one of the previous terms. */
                if (gcd(prev1, (seq_t)cand) != (seq_t)1 ||
                    gcd(prev2, (seq_t)cand) != (seq_t)1) {
                    chosen = (seq_t)cand;
                    break;
                }
            }
        }

        /* Append the chosen term */
        ensure_seq_cap(i + 1, &seq, &seq_cap);
        seq[i] = chosen;
        ++seq_len;

        /* Mark the value as used */
        ensure_used_cap(cand, &used, &used_cap);
        used[(size_t)cand] = 1;
    }

    return seq[n - 1];
}
