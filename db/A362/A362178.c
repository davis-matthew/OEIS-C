/*
 * A362178  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number that has not yet appeared that is a multiple of
 *          omega(a(n-1)).
 * https://oeis.org/A362178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A362178 * Recurrence: a(1)=1, a(2)=2; * for n>2, a(n) = smallest
 *            positive unused integer that is a multiple * of ω(a(n‑1)), where
 *            ω(m) is the number of distinct prime divisors of m
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   count_distinct_primes_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/count_distinct_primes_u64.h"

/* ------------------------------------------------------------ */
/*  Small prime factor count for 64‑bit unsigned integers       */

/* ------------------------------------------------------------ */
/*  Persistent storage for computed terms and used flags        */
/* ------------------------------------------------------------ */
static seq_t *seq_vals = NULL;   /* 1‑based: seq_vals[0] unused */
static size_t seq_cap = 0;       /* maximum stored index (inclusive) */

static uint8_t *used_flag = NULL; /* bitmap: used_flag[x]==1 iff x has appeared */
static size_t used_cap = 0;       /* allocated size (largest index +1) */

/* ------------------------------------------------------------ */
static void ensure_seq_cap(idx_t n)
{
    if (seq_cap >= (size_t)n) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, (new_cap + 1) * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_used_cap(uint64_t v)
{
    if (used_cap > v) return;
    size_t new_cap = used_cap ? used_cap * 2 : 128;
    while (new_cap <= v) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used_flag, new_cap * sizeof(uint8_t));
    if (tmp == NULL) { /* memory failure – let it crash on next use */
        return;
    }
    /* clear new area */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used_flag = tmp;
    used_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Compute terms up to index n                                 */
/* ------------------------------------------------------------ */
static void compute_upto(idx_t n)
{
    if (n <= 0) return;

    /* initialise if this is the first call */
    if (!seq_vals) {
        ensure_seq_cap(2);
        seq_vals[0] = (seq_t)0;          /* unused dummy */
        seq_vals[1] = (seq_t)1;
        seq_vals[2] = (seq_t)2;
        seq_cap = 2;

        ensure_used_cap(2);
        used_flag[1] = 1;
        used_flag[2] = 1;
    }

    /* if already computed, nothing to do */
    if (seq_cap >= (size_t)n) return;

    for (idx_t i = (idx_t)(seq_cap + 1); i <= n; ++i) {
        /* previous term */
        const seq_t prev_val = seq_vals[i - 1];
        uint64_t prev_u = (uint64_t)prev_val;  /* safe for the range needed */
        int omega = count_distinct_primes_u64(prev_u);
        if (omega <= 0) omega = 1; /* safety: ω(1)=0 but not used after seed */

        uint64_t candidate = (uint64_t)omega;
        ensure_used_cap(candidate);
        while (used_flag[candidate]) {
            candidate += (uint64_t)omega;
            ensure_used_cap(candidate);
        }
        used_flag[candidate] = 1;

        ensure_seq_cap(i);
        seq_vals[i] = (seq_t)candidate;
    }
    seq_cap = (size_t)n;
}

/* ------------------------------------------------------------ */
/*  Public interface required by the harness                  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0, guard against UB */
    compute_upto(n);
    return seq_vals[n];
}
