/*
 * A370793  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, a(n) AND a(n+1) equals 0 or
 *          a(n) (where AND denotes the bitwise AND operator).
 * https://oeis.org/A370793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0. For n>0, a(n) is the smallest unused nonnegative integer
   k for which (a(n-1) & k) == 0 or (a(n-1) & k) == a(n-1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* global storage for the sequence and a bitset of used integers */
static seq_t *seq = NULL;       /* array of already generated terms */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* allocated capacity of seq */

static uint64_t *used_bits = NULL;   /* bitset: one bit per integer */
static size_t used_cap = 0;          /* number of 64‑bit words allocated */

/* Ensure seq has capacity for at least cap elements.  Called with cap>seq_cap. */
static void ensure_seq_cap(size_t cap)
{
    if (cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) abort();          /* allocation failure */
    seq = new_seq;
    seq_cap = new_cap;
}

/* Ensure used_bits has room for indices up to ix (inclusive). */
static void ensure_used_cap(size_t ix)
{
    if (ix + 1 <= used_cap) return;     /* enough space already */
    size_t new_cap = used_cap ? used_cap : 64;
    while (new_cap <= ix) new_cap <<= 1;
    uint64_t *new_bits = (uint64_t *)calloc(new_cap, sizeof(uint64_t));
    if (!new_bits) abort();
    if (used_bits) {
        memcpy(new_bits, used_bits, used_cap * sizeof(uint64_t));
        free(used_bits);
    }
    used_bits = new_bits;
    used_cap = new_cap;
}

/* Test whether integer v has been used. */
static bool used_is_used(idx_t v)
{
    idx_t w = v >> 6;
    idx_t b = v & 63;
    ensure_used_cap((size_t)w);
    return (used_bits[w] >> b) & 1ULL;
}

/* Mark integer v as used. */
static void used_set(idx_t v)
{
    idx_t w = v >> 6;
    idx_t b = v & 63;
    ensure_used_cap((size_t)w);
    used_bits[w] |= (1ULL << b);
}

/* Generate terms up to index n (inclusive). */
static void generate_upto(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (seq_len >= need) return;      /* already enough */

    /* initialise first term if sequence is empty */
    if (seq_len == 0) {
        ensure_seq_cap(64);
        seq[0] = (seq_t)0;
        seq_len = 1;
        used_set((idx_t)0);
    }

    while (seq_len < need) {
        seq_t cur = seq[seq_len - 1];
        idx_t candidate = 0;
        seq_t nxt = 0;

        for (;;) {
            if (!used_is_used(candidate)) {
                seq_t cand_t = (seq_t)candidate;
                seq_t res = cur & cand_t;
                if (res == 0 || res == cur) {
                    nxt = cand_t;
                    break;
                }
            }
            ++candidate;
        }

        /* store the next term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = nxt;
        seq_len++;

        used_set((idx_t)nxt);
    }
}

seq_t a(idx_t n)
{
    generate_upto(n);
    return seq[(size_t)n];
}
