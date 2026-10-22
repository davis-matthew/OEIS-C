/*
 * A367917  BII-numbers of set-systems with the same number of edges as
 *          covered vertices.
 * https://oeis.org/A367917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A367917: the BII-numbers of
 * set‑systems with the same number of edges as covered vertices.
 * It tests candidate integers in increasing order; once a
 * candidate satisfies the defining property it is stored
 * in a dynamically growing array and returned for all
 * subsequent queries.                      */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Masks for positions of 1‑bits in i (i <= 64).  mask[i]
 * is a bitmask with bits set exactly at the positions which
 * are 1 in the binary representation of i.  For example,
 * i = 5 (binary 101) => mask[5] = 0b101 (positions 1 and 3). */
static unsigned long long mask[65];
static bool mask_initialised = false;

static void init_mask(void)
{
    if (mask_initialised) return;
    for (int i = 1; i <= 64; ++i) {
        unsigned long long m = 0;
        unsigned long long t = (unsigned long long)i;
        while (t) {
            int p = __builtin_ctzll(t) + 1;   /* position of lowest 1‑bit */
            m |= 1ULL << (p - 1);
            t &= t - 1;                     /* clear lowest 1‑bit */
        }
        mask[i] = m;
    }
    mask_initialised = true;
}

/* Return true if n satisfies the defining property of A367917. */
static bool satisfies(idx_t n)
{
    init_mask();
    if (n == 0) return true;

    unsigned long long union_mask = 0;
    unsigned long long tmp = (unsigned long long)n;
    int edges = 0;

    while (tmp) {
        unsigned long long low = tmp & -tmp;
        int pos = __builtin_ctzll(low) + 1;   /* 1‑indexed position of the bit */
        edges++;
        union_mask |= mask[pos];
        tmp ^= low;
    }
    return edges == __builtin_popcountll(union_mask);
}

/* Dynamically store the sequence terms. */
static seq_t *terms = NULL;
static size_t terms_count = 0;
static size_t terms_capacity = 0;
static idx_t next_candidate = 0;

static void push_term(seq_t v)
{
    if (terms_count >= terms_capacity) {
        size_t new_cap = terms_capacity ? terms_capacity * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* OOM: cannot recover */
        terms = tmp;
        terms_capacity = new_cap;
    }
    terms[terms_count++] = v;
}

/* Public interface.  a(1) is the first term listed in OEIS. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of bounds, safety */

    /* Generate terms until we have at least n terms. */
    while (terms_count < n) {
        if (satisfies(next_candidate))
            push_term((seq_t)next_candidate);
        ++next_candidate;
    }
    return terms[n - 1];
}
