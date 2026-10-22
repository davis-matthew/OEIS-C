/*
 * A341993  a(0)=0. For n > 0, a(n+1) = 2*a(n) if the sum of digits of 2*a(n)
 *          exceeds that of a(n); otherwise, a(n+1) is the smallest unused
 *          nonnegative integer.
 * https://oeis.org/A341993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n>0 let t = a(n-1); a(n) = 2*t if sumdigits(2*t) >
 *            sumdigits(t), otherwise a(n) = the smallest unused non‑negative
 *            integer
 * Range:     u8 n <= 41, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Sequence storage */
static seq_t *seq_tbl = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* Hash table for used values */
static seq_t *used_keys = NULL;
static bool *used_filled = NULL;
static size_t used_cap = 0;
static size_t used_sz  = 0;

/* Next candidate for smallest unused integer */
static idx_t next_unused = 0;

/* Hash function for seq_t values (uses low 64 bits). */
static inline size_t hash_seq_t(seq_t v)
{
    unsigned long long vv = (unsigned long long)v;
    return (size_t)(vv ^ (vv >> 32));
}

/* Ensure seq_tbl has capacity for at least 'needed' elements. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_tbl = (seq_t *)realloc(seq_tbl, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Resize used hash table when load factor > 3/4. */
static void used_resize(void)
{
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    seq_t *new_keys = (seq_t *)calloc(new_cap, sizeof(seq_t));
    bool   *new_filled = (bool *)calloc(new_cap, sizeof(bool));

    for (size_t i = 0; i < used_cap; ++i) {
        if (used_filled[i]) {
            seq_t key = used_keys[i];
            size_t idx = hash_seq_t(key) & (new_cap - 1);
            while (new_filled[idx]) idx = (idx + 1) & (new_cap - 1);
            new_keys[idx] = key;
            new_filled[idx] = true;
        }
    }

    free(used_keys);
    free(used_filled);
    used_keys   = new_keys;
    used_filled = new_filled;
    used_cap    = new_cap;
}

/* Test if a value is already used. */
static bool used_test(seq_t val)
{
    size_t idx = hash_seq_t(val) & (used_cap - 1);
    while (used_filled[idx]) {
        if (used_keys[idx] == val) return true;
        idx = (idx + 1) & (used_cap - 1);
    }
    return false;
}

/* Insert a value into the used hash table. */
static void used_insert(seq_t val)
{
    if ((used_sz + 1) * 4 >= used_cap * 3) used_resize();

    size_t idx = hash_seq_t(val) & (used_cap - 1);
    while (used_filled[idx]) idx = (idx + 1) & (used_cap - 1);
    used_keys[idx]   = val;
    used_filled[idx] = true;
    used_sz++;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* One‑time initialization. */
    if (!seq_tbl) {
        seq_cap = 8;
        seq_tbl = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        seq_tbl[0] = 0;
        seq_len   = 1;

        used_cap   = 16;
        used_keys  = (seq_t *)calloc(used_cap, sizeof(seq_t));
        used_filled = (bool *)calloc(used_cap, sizeof(bool));
        used_insert(0);

        next_unused = 1;
    }

    /* Grow sequence up to required index. */
    if (seq_len <= (size_t)n) {
        seq_t last = seq_tbl[seq_len - 1];
        while (seq_len <= (size_t)n) {
            seq_t candidate = last * 2;
            seq_t val;

            if (digit_sum(candidate) > digit_sum(last))
                val = candidate;
            else
                val = next_unused;

            /* If by chance the chosen value is already used,
               fall back to the smallest unused integer.   */
            if (used_test(val))
                val = next_unused;

            ensure_seq_cap(seq_len + 1);
            seq_tbl[seq_len] = val;

            used_insert(val);

            /* Update next_unused if it was used just now. */
            if (val == next_unused) {
                next_unused++;
                while (used_test(next_unused)) next_unused++;
            }

            last     = val;
            seq_len++;
        }
    }

    return seq_tbl[n];
}
