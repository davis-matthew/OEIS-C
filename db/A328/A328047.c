/*
 * A328047  Permutation of positive integers: a(1) = 1. For n > 1, a(n) is the
 *          smallest positive integer not already in the sequence such that
 *          bigomega(a(n)) != bigomega(a(n-1)), where bigomega(i) is
 *          A001222(i).
 * https://oeis.org/A328047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A328047.
   a(1) = 1. For n > 1 a(n) is the smallest positive integer
   not already used that satisfies bigomega(a(n)) ≠ bigomega(a(n‑1)).
   The function memoizes previously computed terms so that calls in
   arbitrary order and any n ≥ 1 are handled efficiently. 
   It uses the helper function bigomega(idx_t n) supplied in
   ../utils/bigomega.h.
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

/* static state */
static seq_t *seq = NULL;           /* array of a(n) values */
static size_t seq_len = 0;          /* number of terms computed so far */
static size_t seq_cap = 0;          /* capacity of seq array */

static uint8_t *used = NULL;        /* bitmap of used integers: 0=unused,1=used */
static size_t used_cap = 0;         /* capacity of used array (size in bytes) */

/* Ensure seq array can hold at least 'needed' elements. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = new_arr;
    seq_cap = new_cap;
}

/* Ensure used array can index at least 'needed' (i.e. used[0..needed]). */
static void ensure_used_cap(size_t needed)
{
    if (needed < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 128;
    if (new_cap <= needed) {
        while (new_cap <= needed) new_cap *= 2;
    }
    uint8_t *new_arr = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    /* initialise new, unallocated elements to 0 */
    memset(new_arr + used_cap, 0, new_cap - used_cap);
    used = new_arr;
    used_cap = new_cap;
}

/* Compute the next term of the sequence and append it to 'seq'. */
static void compute_next_term(void)
{
    if (seq_len == 0) {
        /* initialise a(1) = 1 */
        ensure_seq_cap(1);
        seq[0] = (seq_t)1;
        ensure_used_cap(2);          /* need index used[1] */
        used[1] = 1;
        seq_len = 1;
        return;
    }

    /* Omega of previous term. */
    seq_t last_val = seq[seq_len - 1];
    idx_t last_omega = (idx_t)bigomega((idx_t)last_val);

    /* Find smallest unused integer whose bigomega differs. */
    idx_t candidate = 1;
    for (;;) {
        ensure_used_cap((size_t)candidate + 1);
        if (!used[(size_t)candidate]) {
            idx_t cand_omega = (idx_t)bigomega(candidate);
            if (cand_omega != last_omega) {
                /* Append to sequence */
                ensure_seq_cap(seq_len + 1);
                seq[seq_len] = (seq_t)candidate;
                used[(size_t)candidate] = 1;
                seq_len++;
                break;
            }
        }
        candidate++;
    }
}

/* Public function requested by the judge. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but avoid UB */
    while (seq_len < (size_t)n) {
        compute_next_term();
    }
    return seq[n - 1];
}
