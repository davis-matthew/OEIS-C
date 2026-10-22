/*
 * A350927  a(1)=1, a(2)=2; for n > 2, a(n) is the smallest unused positive
 *          number such that gcd(a(n-1) * |a(n-1) - a(n-2)|, a(n)) > 1.
 * https://oeis.org/A350927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2; for n>2, a(n) is the smallest unused positive
   integer such that gcd( a(n-1) * |a(n-1)-a(n-2)| , a(n) ) > 1.
   The implementation stores the sequence in a dynamically resized array
   and marks used values in a bit array. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* global state */
static seq_t   *seq      = NULL;   /* sequence values */
static idx_t   seq_len  = 0;       /* number of terms computed */
static idx_t   seq_cap  = 0;       /* capacity of seq array */

static unsigned char *used      = NULL;   /* bit array of used numbers (0/1) */
static size_t        used_cap   = 0;      /* capacity of used array */

/* Ensure seq has capacity at least "need" */
static void ensure_seq_capacity(idx_t need) {
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure used array can index value "val" */
static void ensure_used_capacity(seq_t val) {
    uint64_t ix = (uint64_t)val;
    if (ix + 1 > used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 1024;
        while (new_cap <= ix) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
        if (!tmp) abort();
        /* zero new part */
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* initialise if first call */
    if (seq_len == 0) {
        ensure_seq_capacity(4);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        ensure_used_capacity(2);
        used[(size_t)1] = 1;
        used[(size_t)2] = 1;
        seq_len = 2;
    }

    if (n <= seq_len) return seq[n-1];

    /* generate terms until we reach n */
    while (seq_len < n) {
        seq_t last        = seq[seq_len-1];
        seq_t second_last = seq[seq_len-2];
        seq_t diff         = (last >= second_last) ? (last - second_last)
                                                    : (second_last - last);

        seq_t cand;
        for (cand = 1;; cand++) {
            ensure_used_capacity(cand);
            size_t idx = (size_t)(uint64_t)cand;
            if (!used[idx]) {
                seq_t g1 = gcd(last, cand);
                if (g1 > 1) break;
                seq_t g2 = gcd(diff, cand);
                if (g2 > 1) break;
            }
        }

        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = cand;
        used[(size_t)(uint64_t)cand] = 1;
        seq_len++;
    }

    return seq[n-1];
}
