/*
 * A339723  Start with a(1)=1. Thereafter, to get a(n), write a(n-1) in
 *          binary, write down all its run lengths, and concatenate them in
 *          binary, getting k. Then a(n) = k unless k is already in the
 *          sequence, in which case a(n) = smallest missing number.
 * https://oeis.org/A339723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1. Recurrence (n > 1): Write a(n‑1) in binary, find the
 *            run lengths of consecutive identical bits, concatenate those run
 *            lengths in binary to obtain k. If k has not appeared earlier in
 *            the sequence let a(n) = k; otherwise a(n) is the smallest
 *            positive integer not yet in the sequence
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* -------------  static state  ---------------------------------- */
static seq_t   *seq          = NULL;      /* sequence values        */
static idx_t   seq_cap       = 0;         /* capacity of seq        */
static idx_t   seq_len       = 0;         /* number of terms stored */

static bool    *used         = NULL;      /* table of used numbers  */
static idx_t   used_cap      = 0;         /* capacity of used table */
static idx_t   next_missing  = 1;         /* smallest unused integer */

/* -------------  helper to grow the used table  ------------------ */
static void ensure_used_cap(idx_t need)
/* enlarge used[] so that it may index up to 'need' */
{
    if (need < used_cap) return;
    idx_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= need) new_cap *= 2;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(1);                     /* fatal error */
    for (idx_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
    used      = tmp;
    used_cap  = new_cap;
}

/* -------------  helper to grow the sequence array  --------------- */
static void ensure_seq_cap(idx_t need)
/* enlarge seq[] so that it may store 'need' elements           */
{
    if (need < seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap <= need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq   = tmp;
    seq_cap = new_cap;
}

/* -------------  main sequence function  ------------------------ */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq == NULL) {
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = (seq_t)1;          /* a(1) = 1 */
        seq_len = 1;

        used_cap = 32;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!used) exit(1);
        used[1] = true;             /* 1 is used */
        next_missing = 2;
    }

    /* generate the sequence up to n */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];

        /* build k by concatenating run‑lengths of prev */
        seq_t k = (seq_t)0;
        idx_t bl = (idx_t)bitlen_seq_t(prev);
        if (bl == 0) bl = 1;                   /* safety – never true */
        idx_t msb = bl - 1;
        bool cur_bit = ((prev >> msb) & (seq_t)1) != 0;
        idx_t run_len = 1;

        for (idx_t pos = msb; pos > 0; --pos) {
            bool bit = ((prev >> (pos - 1)) & (seq_t)1) != 0;
            if (bit == cur_bit) {
                ++run_len;
            } else {
                idx_t lenbits = (idx_t)bitlen_seq_t((seq_t)run_len);
                k = (k << lenbits) | (seq_t)run_len;
                cur_bit = bit;
                run_len = 1;
            }
        }
        /* last run */
        idx_t lenbits = (idx_t)bitlen_seq_t((seq_t)run_len);
        k = (k << lenbits) | (seq_t)run_len;

        /* decide the term */
        bool seen = false;
        for (idx_t i = 0; i < seq_len; ++i)
            if (seq[i] == k) { seen = true; break; }

        seq_t term;
        if (!seen) {                      /* k is new */
            term = k;
        } else {                          /* find missing integer */
            idx_t cand = next_missing;
            while (1) {
                ensure_used_cap(cand);
                if (!used[cand]) break;
                ++cand;
            }
            term = cand;
            ++next_missing;                /* advance for next search */
        }

        /* store the new term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = term;

        /* mark the term as used */
        ensure_used_cap((idx_t)term);
        used[(idx_t)term] = true;

        /* adjust next_missing if it was just consumed */
        while (used[next_missing]) ++next_missing;
    }

    return seq[n - 1];
}
