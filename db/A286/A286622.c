/*
 * A286622  Restricted growth sequence computed for filter-sequence A278222,
 *          related to 1-runs in the binary representation of n.
 * https://oeis.org/A286622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 9686, u16 n <= 9686, u32 n <= 9686, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   compute_1run_multiset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A286622: assigns a unique increasing integer to each distinct
   multiset of lengths of consecutive 1‑runs in the binary representation
   of n.  The empty multiset (n = 0) receives value 1 and each new multiset
   receives the next integer label. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/compute_1run_multiset.h"

/* Helper data structure for each distinct multiset */
typedef struct {
    size_t len;           /* number of runs                     */
    uint8_t *lens;        /* sorted array of run lengths        */
    seq_t label;          /* assigned label                     */
} Key;

/* Static cache of computed labels: label_cache[n] = a(n) */
static seq_t *label_cache = NULL;
static size_t label_cap   = 0;      /* current capacity          */
static idx_t  max_generated = 0;    /* largest n that has been generated */

/* List of distinct multisets encountered */
static Key *keys      = NULL;
static size_t key_cap = 0;
static size_t key_cnt = 0;

/* Next unused label value */
static seq_t next_label = 1;

/* -------------------------------------------------------------------------- */
/* Ensure that label_cache can hold indices up to n (inclusive). */
static void ensure_label_cache(idx_t n)
{
    if (label_cache && (size_t)n < label_cap)
        return;

    size_t new_cap = label_cap ? label_cap << 1 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(label_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                   /* out of memory */

    /* Initialise any newly allocated entries to zero */
    for (size_t i = label_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    label_cache = tmp;
    label_cap   = new_cap;
}

/* Ensure that the key list has space for another entry. */
static void ensure_key_cap(void)
{
    if (key_cnt < key_cap)
        return;

    size_t new_cap = key_cap ? key_cap << 1 : 8;
    Key *tmp = (Key *)realloc(keys, new_cap * sizeof(Key));
    if (!tmp)
        abort();           /* out of memory */

    keys      = tmp;
    key_cap   = new_cap;
}

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Generate labels for all n between max_generated + 1 and n (inclusive). */
static void generate_up_to(idx_t n)
{
    for (idx_t i = max_generated + 1; i <= n; ++i) {
        /* Compute the multiset for this i. */
        size_t len;
        uint8_t tmp_lens[64];
        compute_1run_multiset((uint64_t)i, &len, tmp_lens);

        /* Search existing keys. */
        seq_t label = 0;
        for (size_t k = 0; k < key_cnt; ++k) {
            if (keys[k].len != len)
                continue;
            if (len == 0 || memcmp(keys[k].lens, tmp_lens, len) == 0) {
                label = keys[k].label;
                break;
            }
        }

        /* If not found, create a new key. */
        if (label == 0) {
            ensure_key_cap();
            Key *kp = &keys[key_cnt++];
            kp->len = len;
            if (len) {
                kp->lens = (uint8_t *)malloc(len);
                if (!kp->lens)
                    abort();
                memcpy(kp->lens, tmp_lens, len);
            } else {
                kp->lens = NULL;
            }
            kp->label = next_label++;
            label = kp->label;
        }

        /* Store label. */
        label_cache[(size_t)i] = label;
    }
    max_generated = n;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Lazy initialisation of the cache. */
    if (!label_cache) {
        ensure_label_cache(n);
        label_cache[0] = (seq_t)1;   /* empty multiset gets 1 */
        next_label = (seq_t)2;
        max_generated = 0;
    }

    if (n > max_generated) {
        ensure_label_cache(n);
        generate_up_to(n);
    }
    return label_cache[(size_t)n];
}
