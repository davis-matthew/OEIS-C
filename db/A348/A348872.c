/*
 * A348872  After a(1) = 1, the sequence is always extended with the smallest
 *          divisor d (not yet present in the sequence) of the last term t. If
 *          d doesn't exist, we extend the sequence with 2*t + 1 and repeat.
 *          See the Comments section for more details.
 * https://oeis.org/A348872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A348872
 *
 *   a(1) = 1;
 *   For n > 1, let t = a(n-1).  If t has a divisor d that has not
 *   appeared before in the sequence, a(n) is that smallest such d.
 *   If no such divisor exists, a(n) = 2*t + 1.
 *
 *   The implementation memoises the sequence up to the largest
 *   index requested and keeps a hash table of all already seen
 *   terms for fast membership tests.  This allows any arbitrary order
 *   of calls to a(idx_t) and works for n up to about 10 000 within
 *   the time limits. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* forward declarations of helper functions */
static bool _seen(seq_t value);
static void _insert(seq_t value);
static void _grow_seq(size_t needed);
static void _init_hash(void);
static inline size_t _hash_val(seq_t v);

/* static storage for the sequence and the hash table */
static seq_t   *seq_arr   = NULL;   /* array of sequence terms          */
static size_t   seq_len   = 0;      /* elements presently stored       */
static size_t   seq_cap   = 0;      /* current capacity of seq_arr      */

static seq_t   *hash_arr  = NULL;   /* hash table for seen values      */
static size_t   hash_cap  = 0;      /* capacity of hash_arr (power of 2) */

/* ------------------------------------------------------------------ */
/*      hash table (open addressing, linear probing)                  */
/* ------------------------------------------------------------------ */
#define HASH_INITIAL_CAP (1U << 16)   /* 65536 entries – more than enough */

static void _init_hash(void)
{
    hash_cap = HASH_INITIAL_CAP;
    hash_arr = (seq_t*)calloc(hash_cap, sizeof(seq_t));
}

static inline size_t _hash_val(seq_t v)
{
    /* 64‑bit hash from 128‑bit value (if present) */
    uint64_t hv;
    if (sizeof(seq_t) <= 8) {
        hv = (uint64_t)v;
    } else {
        hv = (uint64_t)v ^ (uint64_t)(v >> 64);
    }
    hv ^= hv >> 33;
    hv *= 0xff51afd7ed558ccdULL;
    hv ^= hv >> 33;
    return hv & (hash_cap - 1);
}

static bool _seen(seq_t v)
{
    size_t h = _hash_val(v);
    for (;;) {
        if (hash_arr[h] == 0) return false;    /* empty slot */
        if (hash_arr[h] == v)   return true;    /* found */
        h = (h + 1) & (hash_cap - 1);
    }
}

static void _insert(seq_t v)
{
    size_t h = _hash_val(v);
    for (;;) {
        if (hash_arr[h] == 0) {                /* empty slot */
            hash_arr[h] = v;
            return;
        }
        if (hash_arr[h] == v) return;          /* already present */
        h = (h + 1) & (hash_cap - 1);
    }
}

/* ------------------------------------------------------------------ */
/*      dynamic array for the sequence                                 */
/* ------------------------------------------------------------------ */
static void _grow_seq(size_t needed)
{
    if (seq_arr == NULL) {
        seq_cap = 16;
        seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }
    while (seq_cap < needed) {
        seq_cap <<= 1;
        seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------ */
/*      Main function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is undefined for n < 1 */

    /* initialise storage on first call */
    if (seq_len == 0) {
        _grow_seq(n);            /* allocate space for at least n elements */
        _init_hash();            /* initialise hash table for seen values */

        /* a(1) = 1 */
        seq_arr[0] = (seq_t)1;
        seq_len = 1;
        _insert((seq_t)1);
    }

    /* extend the sequence up to index n */
    while (seq_len < (size_t)n) {
        seq_t t = seq_arr[seq_len - 1];
        seq_t min_d = 0;      /* store smallest divisor not yet seen */

        /* compute integer sqrt to bound the divisor loop */
        seq_t sqrt_t = floor_sqrt(t);
        idx_t limit = (sqrt_t > (seq_t)UINT64_MAX) ? UINT64_MAX : (idx_t)sqrt_t;

        for (idx_t d = 1; d <= limit; ++d) {
            if (t % (seq_t)d == 0) {
                seq_t d_val = (seq_t)d;
                if (! _seen(d_val)) {
                    if (min_d == 0 || d_val < min_d) min_d = d_val;
                }
                seq_t e_val = t / d_val;
                if (e_val != d_val && ! _seen(e_val)) {
                    if (min_d == 0 || e_val < min_d) min_d = e_val;
                }
            }
        }

        seq_t next = (min_d == 0) ? ((seq_t)2 * t + 1) : min_d;

        /* store the new term */
        _grow_seq(seq_len + 1);
        seq_arr[seq_len] = next;
        _insert(next);
        seq_len++;
    }

    return seq_arr[(size_t)n - 1];
}
