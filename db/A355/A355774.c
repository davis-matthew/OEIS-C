/*
 * A355774  An extension of the generalized pentagonal numbers such that every
 *          positive integer can be represented as the sum of at most two
 *          terms of the sequence.
 * https://oeis.org/A355774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355774: a(0)=0; thereafter the terms are constructed inductively.
   A term m is added to the sequence iff
   (i) m is a generalized pentagonal number, or
   (ii) m cannot be expressed as the sum of two earlier terms.
   This implementation keeps a dynamic array of the terms and a
   dynamic bitmap of representable sums, building the sequence lazily
   so that any call a(n) (in any order) is efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq = NULL;      /* a(0), a(1), … stored */
static size_t seq_len = 0;     /* number of stored terms */
static size_t seq_cap = 0;     /* capacity of seq array */

static bool *rep = NULL;       /* rep[i] true iff i is a sum of two earlier terms */
static size_t rep_cap = 0;     /* capacity of rep array (for indices 0..cap-1) */

static idx_t next_candidate = 0;   /* next integer to consider */

/* Ensure seq can hold at least `need` elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_seq) return;    /* allocation failure: caller will crash */
    if (seq) {
        memcpy(new_seq, seq, seq_len * sizeof(seq_t));
        free(seq);
    }
    seq = new_seq;
    seq_cap = new_cap;
}

/* Ensure rep can address indices up to `idx`. Newly allocated entries are zeroed. */
static void ensure_rep_capacity(idx_t idx)
{
    if ((idx_t)rep_cap > idx) return;
    size_t new_cap = rep_cap ? rep_cap * 2 : 8;
    while ((idx_t)new_cap <= idx) new_cap <<= 1;
    bool *new_rep = (bool *)calloc(new_cap, sizeof(bool));
    if (!new_rep) return;     /* allocation failure */
    if (rep) {
        memcpy(new_rep, rep, rep_cap * sizeof(bool));
        free(rep);
    }
    rep = new_rep;
    rep_cap = new_cap;
}

/* Test whether m is a generalized pentagonal number. */
static bool is_generalized_pentagonal(seq_t m)
{
    seq_t D = (seq_t)24 * m + 1;
    seq_t s = floor_sqrt(D);
    if (s * s != D) return false;
    seq_t mod6 = s % 6;
    return (mod6 == 1) || (mod6 == 5);
}

/* Test whether number m can be written as the sum of two earlier terms. */
static bool is_representable(idx_t m)
{
    ensure_rep_capacity(m);
    return rep[m];
}

/* Append a new term m to the sequence and update representable sums. */
static void add_term(seq_t m)
{
    size_t old_len = seq_len;
    ensure_seq_capacity(seq_len + 1);
    seq[seq_len++] = m;

    /* Update rep for sums involving m and all previous terms. */
    for (size_t i = 0; i < old_len; ++i) {
        idx_t sum = (idx_t)(seq[i] + m);
        ensure_rep_capacity(sum);
        rep[sum] = true;
    }
    /* Add m + m as well. */
    idx_t sum = (idx_t)(m + m);
    ensure_rep_capacity(sum);
    rep[sum] = true;
}

/* Generate sequence terms until at least n+1 terms are stored. */
static void generate_up_to(idx_t n)
{
    while (seq_len <= n) {
        seq_t m = (seq_t)next_candidate;
        if (is_generalized_pentagonal(m) || !is_representable((idx_t)m)) {
            add_term(m);
        }
        ++next_candidate;
    }
}

/* Public entry point: return a(n). */
seq_t a(idx_t n)
{
    generate_up_to(n);
    return seq[n];
}
