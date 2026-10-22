/*
 * A386482  a(1)=1, a(2)=2; thereafter a(n) is either the greatest number k <
 *          a(n-1) not already used such that gcd(k, a(n-1)) > 1, or if no
 *          such k exists then a(n) is the smallest number k > a(n-1) not
 *          already used such that gcd(k, a(n-1)) > 1.
 * https://oeis.org/A386482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386482
   a(1) = 1, a(2) = 2;
   thereafter a(n) is the greatest unused k < a(n-1) with gcd(k,a(n-1))>1,
   or, if none exists, the smallest unused k > a(n-1) with gcd(k,a(n-1))>1.
   The following function memoises the sequence and works for arbitrary
   integer orders in C11 or C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

/* static storage for memoisation */
static seq_t *seq = NULL;          /* sequence values */
static size_t seq_cap = 0;         /* capacity of seq  */
static size_t seq_len = 0;         /* number of terms stored */

static uint8_t *used = NULL;       /* 0 = unused, 1 = used */
static size_t used_cap = 0;        /* capacity of used (inclusive of max index) */

/* Ensure seq has space for at least 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure used has space up to index 'need' (inclusive) */
static void ensure_used_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    if (new_cap > used_cap)
        memset(tmp + used_cap, 0, new_cap - used_cap);  /* initialise new slots */
    used = tmp;
    used_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* undefined behaviour for n == 0; return 0 */
    if (n == 0) return 0;

    /* first call initialisation */
    if (!seq) {
        /* initialise capacity for at least two terms */
        ensure_seq_cap(2);
        seq[0] = (seq_t)1;   /* a(1) */
        seq[1] = (seq_t)2;   /* a(2) */
        seq_len = 2;

        /* initialise used set */
        ensure_used_cap(2);
        used[1] = 1;
        used[2] = 1;
    }

    /* if the term is already computed, return it */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* compute terms up to n */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        seq_t cand = (seq_t)0;
        bool found = false;

        /* search downward for the greatest unused k < prev */
        if (prev > (seq_t)1) {
            for (seq_t down = prev - 1; down >= (seq_t)1; --down) {
                size_t idx = (size_t)down;
                ensure_used_cap(idx);
                if (!used[idx] && gcd(prev, down) != (seq_t)1) {
                    cand = down;
                    found = true;
                    break;
                }
                /* for unsigned types the loop stops when down reaches 0 automatically */
            }
        }

        /* if none found, search upward */
        if (!found) {
            for (seq_t up = prev + 1;; ++up) {
                size_t idx = (size_t)up;
                ensure_used_cap(idx);
                if (!used[idx] && gcd(prev, up) != (seq_t)1) {
                    cand = up;
                    break;
                }
            }
        }

        /* append new term and mark it used */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = cand;
        size_t uidx = (size_t)cand;
        ensure_used_cap(uidx);
        used[uidx] = 1;
    }

    return seq[(size_t)n - 1];
}
