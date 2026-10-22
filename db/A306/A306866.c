/*
 * A306866  Lexicographically earliest sequence of distinct positive terms
 *          such that the binary representation of the sum of two consecutive
 *          terms has at most 2 runs of ones.
 * https://oeis.org/A306866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   runs_of_ones_leq2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Guaranteed implementation of OEIS A306866:  lexicographically
 *  earliest sequence of distinct positive terms such that the binary
 *  representation of the sum of two consecutive terms has at most
 *  two runs of ones.  The sequence is generated greedily:  each
 *  a(n) is the smallest unused positive integer that keeps the
 *  "at most two runs of ones" condition with a(n‑1).  Caching is
 *  used so that any n ≥ 1 can be answered in O(n) time and the
 *  function works for all supported integer types.  */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/runs_of_ones_leq2.h"

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Boolean array marking which numbers have already been used.
 *  used[i] == 1  ⇔  value i has appeared somewhere in the sequence. */
static unsigned char *used = NULL;
static idx_t used_cap = 0;   /* capacity of used array (in indices) */

/* Ensure that the seq array can hold at least 'need' elements. */
static void ensure_seq_capacity(size_t need) {
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
#else
    new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    for (size_t i = 0; i < seq_len; ++i) new_arr[i] = seq[i];
#ifdef __cplusplus
    delete[] seq;
#else
    free(seq);
#endif
    seq = new_arr;
    seq_cap = new_cap;
}

/* Ensure that the used array can index up to 'need' (inclusive). */
static void ensure_used_capacity(idx_t need) {
    if (used_cap > need) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while ((idx_t)new_cap <= need) new_cap <<= 1;
    unsigned char *new_used = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
    if (!new_used) abort();          /* out of memory */
    /* initialize new slots to 0 */
    memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
    used = new_used;
    used_cap = (idx_t)new_cap;
}

/* Generate the sequence up to index n (1‑based).  The sequence is
 * stored in the static array 'seq'.  Duplicate numbers are avoided
 * via the 'used' table. */
static void generate_up_to(idx_t n) {
    if (seq_len >= (size_t)n) return;
    while (seq_len < (size_t)n) {
        if (seq_len == 0) {          /* first term */
            ensure_seq_capacity(1);
            seq[0] = (seq_t)1;
            seq_len = 1;
            ensure_used_capacity(1);
            used[1] = 1;
            continue;
        }
        seq_t prev = seq[seq_len - 1];
        idx_t cand_idx = 1;
        for (;;) {
            ensure_used_capacity(cand_idx);
            if (used[cand_idx]) { ++cand_idx; continue; }
            seq_t cand = (seq_t)cand_idx;
            seq_t sum = prev + cand;
            if (runs_of_ones_leq2(sum)) {
                ensure_seq_capacity(seq_len + 1);
                seq[seq_len] = cand;
                seq_len++;
                used[cand_idx] = 1;
                break;
            }
            ++cand_idx;
        }
    }
}

/* Public interface required by the build system.  a(n) returns the
 * n‑th term of the sequence (1‑based).  For n < 1 the function
 * returns 0 as a harmless dummy value. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1 */
    generate_up_to(n);
    return seq[n - 1];
}
