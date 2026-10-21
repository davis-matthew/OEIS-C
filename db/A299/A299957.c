/*
 * A299957  The sum a(n) + a(n+1) always has at least one digit "1".
 *          Lexicographically first such sequence of nonnegative integers
 *          without duplicate term.
 * https://oeis.org/A299957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  lexicographically first permutation of the non‑negative integers
   such that for every n, the decimal representation of a(n)+a(n+1) contains
   at least one digit ‘1’.  The sequence is constructed greedily:
   a(0)=0 and for n>0
      a(n) = the smallest unused non‑negative integer k with
             digit_1(a(n-1)+k)=true   (digit_1(x) is true iff x has a ‘1’).
   This algorithm generates exactly the terms listed in OEIS A299957. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Helpers for dynamic arrays ---------- */
static void *grow(void *ptr, size_t *cap, size_t elem_size, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? (*cap << 1) : 8;
        while (new_cap < needed) new_cap <<= 1;
        ptr = realloc(ptr, new_cap * elem_size);
        if (!ptr) abort();            /* out of memory */
        /* initialise new memory if array type is bool */
        if (elem_size == sizeof(bool)) {
            size_t i;
            for (i = *cap; i < new_cap; ++i) ((bool *)ptr)[i] = false;
        }
        *cap = new_cap;
    }
    return ptr;
}

static bool has_digit_one(seq_t v)
{
    if (v == 0) return false;
    while (v > 0) {
        if ((v % 10) == 1) return true;
        v /= 10;
    }
    return false;
}

/* ---------- Static storage ---------- */
static seq_t   *seq       = NULL;   /* array of a(n) values */
static idx_t    seq_len   = 0;      /* number of terms stored      */
static idx_t    seq_cap   = 0;      /* allocated capacity          */

static bool     *used      = NULL;   /* used[k] == true  => k used  */
static idx_t    used_cap   = 0;      /* allocated capacity for used */

/* ---------- The sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq_len == 0) {
        seq    = (seq_t *)   realloc(seq, sizeof(seq_t));
        seq_cap = 1;
        seq[0] = 0;
        seq_len = 1;

        used     = (bool *)  realloc(used, sizeof(bool));
        used_cap = 1;
        used[0]  = true;
    }

    /* extend sequence up to index n (any order of calls is allowed) */
    while (seq_len <= n) {
        seq_t prev = seq[seq_len - 1];
        idx_t  cand = 0;

        /* search for smallest unused integer satisfying the property */
        while (true) {
            /* make sure used array covers candidate */
            used = (bool *)grow(used, &used_cap, sizeof(bool), cand + 1);
            if (!used[cand]) {
                seq_t sum = prev + (seq_t)cand;
                if (has_digit_one(sum)) {
                    /* store new term */
                    seq = (seq_t *)grow(seq, &seq_cap, sizeof(seq_t), seq_len + 1);
                    seq[seq_len] = (seq_t)cand;
                    used[cand] = true;
                    ++seq_len;
                    break;
                }
            }
            ++cand;
        }
    }

    return seq[n];
}
