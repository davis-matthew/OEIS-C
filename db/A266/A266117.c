/*
 * A266117  Lexicographically first injection of positive integers beginning
 *          with a(1) such that a(n)*a(n+1) is a term of A265349, i.e., has no
 *          multiple occurrences of any nonzero digit when viewed in factorial
 *          base (A007623).
 * https://oeis.org/A266117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 408, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   good_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(1)=1. For n>1 let p=a(n-1).  
   a(n) is the smallest unused positive integer k such that the product
   p*k has a factorial‑base representation with every non‑zero digit
   occurring at most once.  The routine keeps the sequence and a
   parallel “used” bitmap in static storage and grows them lazily. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../utils/good_product.h"

/* ==================== dynamic storage helpers ==================== */

static seq_t *seq     = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static bool *used     = NULL;
static size_t used_cap = 0;

static uint64_t next_min = 1;   /* smallest integer not yet used */

static void ensure_cap_seq(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t newcap = seq_cap ? (seq_cap << 1) : 16;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = newcap;
}

static void ensure_cap_used(size_t needed)
{
    if (used_cap >= needed) return;
    size_t newcap = used_cap ? (used_cap << 1) : 32;
    if (newcap < needed) newcap = needed;
    bool *tmp = (bool *)realloc(used, newcap * sizeof(bool));
    if (!tmp) abort();
    /* initialise the newly allocated entries to false */
    memset(tmp + used_cap, 0, (newcap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = newcap;
}

/* ==================== factorial‑base uniqueness test ==================== */

/* ==================== public sequence function ==================== */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of offset */

    /* generate terms up to the requested index */
    while (seq_len < (size_t)n) {
        if (seq_len == 0) {                /* initialise a(1) */
            ensure_cap_seq(1);
            seq[0] = (seq_t)1;
            seq_len = 1;

            ensure_cap_used(2);            /* indices 0 and 1 may be needed */
            used[1] = true;
            next_min = 2;
            continue;
        }

        seq_t prev = seq[seq_len - 1];
        uint64_t cand = next_min;

        /* find the smallest unused candidate satisfying the condition */
        while (1) {
            if (cand >= used_cap) ensure_cap_used(cand + 1);
            if (!used[cand] && good_product(prev, (seq_t)cand)) {
                break;   /* accept cand */
            }
            ++cand;
        }

        /* store the accepted term */
        ensure_cap_seq(seq_len + 1);
        seq[seq_len] = (seq_t)cand;
        seq_len++;

        /* mark cand as used */
        if (cand >= used_cap) ensure_cap_used(cand + 1);
        used[cand] = true;

        /* advance next_min to the next unused integer */
        while (1) {
            if (next_min >= used_cap) ensure_cap_used(next_min + 1);
            if (!used[next_min]) break;
            ++next_min;
        }
    }

    return seq[n - 1];
}
