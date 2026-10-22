/*
 * A363872  Lexicographically earliest sequence of distinct terms > 0 such
 *          that n is a substring of a(n) + a(n+1).
 * https://oeis.org/A363872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Series: Lexicographically earliest sequence of distinct positive integers
 *         such that for every n ≥ 1, the decimal representation of n
 *         occurs as a substring of a(n)+a(n+1).
 *         The sequence is built by a greedy construction that respects
 *         the constraint from the previous index.
 *
 * Implementation note: the function a() is memoised – it builds the
 * sequence incrementally and can be called for arbitrary n in any order.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/seq_to_str.h"

/* Array of sequence values: 1‑based indexing (seq[1] is a(1)) */
static seq_t *seq      = NULL;
static size_t seq_cap  = 0;
static idx_t  seq_len  = 0;        /* highest index computed, 0 means none */

/* Array of values already used in the sequence */
static seq_t *used_list  = NULL;
static size_t used_len   = 0;
static size_t used_cap   = 0;

/* Ensure seq array has room for indices up to n (1‑based). */
static void ensure_seq_cap(idx_t n)
{
    if (seq_cap >= (size_t)n + 1) return;   /* +1: we may store a(n+1) */
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap <= (size_t)n + 1) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Linear search for membership in used_list.  For the expected
 * problem size this is fast enough. */
static bool is_used(seq_t v)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_list[i] == v) return true;
    return false;
}

/* Record a new used value. */
static void record_used(seq_t v)
{
    if (used_len == used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 64;
        used_list = (seq_t *)realloc(used_list, new_cap * sizeof(seq_t));
        used_cap  = new_cap;
    }
    used_list[used_len++] = v;
}

/* Test whether decimal string of n_int occurs in decimal string of sum. */
static bool contains_substring(idx_t n_int, seq_t sum)
{
    char nbuf[64], sbuf[128];
    size_t nlen, slen;

    seq_to_str((seq_t)n_int, nbuf, &nlen);
    seq_to_str(sum, sbuf, &slen);
    nbuf[nlen] = '\0';
    sbuf[slen] = '\0';

    /* Standard strstr works on C null‑terminated strings. */
    return strstr(sbuf, nbuf) != NULL;
}

/* Build the sequence up to index n (inclusive). */
static void compute_until(idx_t n)
{
    while (seq_len < n) {
        /* First pair (indices 1,2): special because there is no left
         * neighbour constraint.  Afterwards we step in units of 2. */
        if (seq_len == 0) {     /* first pair */
            ensure_seq_cap(2);
            for (idx_t a1 = 1; ; ++a1) {
                if (is_used(a1)) continue;
                for (idx_t a2 = 1; ; ++a2) {
                    if (a2 == a1 || is_used(a2)) continue;
                    seq_t sum = (seq_t)a1 + (seq_t)a2;
                    if (contains_substring(1, sum)) {
                        seq[1] = (seq_t)a1;
                        seq[2] = (seq_t)a2;
                        record_used((seq_t)a1);
                        record_used((seq_t)a2);
                        seq_len = 2;
                        break;          /* exit inner loop */
                    }
                }
                if (seq_len == 2) break;     /* outer loop exited */
            }
            continue;   /* proceed to next outer iteration */
        }

        /* Continue with pairs starting at indices 2,4,6,...            */
        idx_t cur = seq_len + 1;          /* position of the left element of the new pair */
        if (cur > n) break;              /* no more indices needed */

        ensure_seq_cap(cur + 1);          /* need room for a(cur) and a(cur+1) */

        seq_t a_prev = seq[cur - 1];      /* left neighbour for the left element of the pair */

        for (idx_t cand = 1; ; ++cand) {
            if (is_used(cand)) continue;

            seq_t sum_left = a_prev + (seq_t)cand;
            if (!contains_substring(cur - 1, sum_left)) continue;

            /* Now find the smallest unused a_next which completes the pair. */
            for (idx_t next = 1; ; ++next) {
                if (next == cand || is_used(next)) continue;
                seq_t sum_right = (seq_t)cand + (seq_t)next;
                if (!contains_substring(cur, sum_right)) continue;

                /* We have a valid pair.  Store it and break. */
                seq[cur]     = (seq_t)cand;
                seq[cur + 1] = (seq_t)next;
                record_used((seq_t)cand);
                record_used((seq_t)next);
                seq_len = cur + 1;
                break;      /* break inner loop: found a pair */
            }
            if (seq_len == cur + 1) break;  /* pair found, exit outer loop */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined value, safe return */

    if (seq_len < n) compute_until(n);
    return seq[(size_t)n];
}
